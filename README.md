# flexion_viga
 
  El firmware se encuentra en el estadio de investigacion.

Objetivos
El objetivo del documento es describir las funcionalidades y componentes que tiene el
firmware.
Para una mejor comprensión del firmware se describe el equipo mecánico que hay que
controlar y el hardware de la placa electrónica.

Equipo mecánico.

El sistema está compuesto  de dos motores paso a paso, el motor1 se encarga de mover el motor 2, a la distancia  donde se aplica la fuerza del ensayo, y el motor2 es el que aplica dicha fuerza.

El motor1  esta controlado a lazo abierto.
El motor2 esta controlado a lazo cerrado con una celda de carga, que llamamos cell_force.

Cada motor tiene un final de carrera para poder tomar una referencia de movimiento.

En los extremos de la viga tenemos dos celdas de cargas que miden las fuerzas de reacción, estas las llamamos   cell_reaction1, cell_reaction2 .

En el medio de la viga y por debajo contamos con un sensor TOF el VL6180x, este se encarga de medir la   medir la flexión de la viga en el punto central.
 
Placa electrónica

La alimentación es de 24v  para los motores Dm542E, y de 5V para el resto del equipo.

El core del equipo es un microcontrolador ATmega2560 de 256Kbytes de Flash, 8K de SRAM y 4K bytes de EEPROM de la empresa Microchip. Montado sobre la plataforma Arduino mega. Usa un cristal de 14.675Mhz para cubrir los baudarate estándar de RS232.

El control de los motores  se hace con un puente H integrado al driver DM8606 .
El uC lo controla por tres  pines STEP,DIR, ENAB  que le permiten regular la velocidad, modificar el sentido de giro, y habilitar y deshabilitar.

Controla dos finales de carrera. 

Posee tres (3)  celdas de carga para medir fuerza. Dos (2) celdas  de  10 kg y una de 50Kg , las tres poseen el HX771 ,encargado de acondicionar la señal de las galgas  internas a cada celda. Usa un conversor A/D dedicado de 24 bits, el cual devuelve un valor proporcional, a la deformación, por un protocolo serie 

Posee un sensor del tipo TOF que se encarga de medir al flexión  ,este sensor tiene un alcance de 20cm, y se controla por medio de IC2.

Para comunicarse con el host se utiliza un puerto serie USB que emula  RS232.

Posicion.

El motor 1 a través de un acople mecánico es solidario a una barra roscada M8, de 2m por paso, esta tiene acoplado al motor 2. Sobre la viga el motor 2  tiene un desplazamiento útil de 20.6cm 50 cm
 

Para establecer la posición del ensayo, se inicializa el punto de referencia cero, con el limite de carrera del motor1 y luego se mueve la cantidad de pasos necesarios para llegar a la distancia pedida.



 Funcionalidades genéricas

El firmware consta con lo siguiente sub – sistemas 

Configuración

Administra los valores de configuración en la Eeprom.


Logeo
Dependiendo de un parámetro  de configuración  log_level, administra el logeo por el puerto serie del equipo. 

// {log_level:'0'}       log_level:0=desactivado,
// {log_level:'1'}                 1=mensajes.
// {log_level:'2'}                 2=info control estandar.
// {log_level:'3'}                 3=info control arduino plotter.


Timer

Para poder medir lapsos de tiempo, implementa el uso de timer por firmware.

Button 

Da soporte al debounce no bloqueancte de los finales de carrera.

Motor
Da soporte al movimiento de los motores 

Celda de carga.

Da soporte al las celdas de cargas.

Tof

Da soporte al sensor Tof

Descripción del firmware

A continuación se describen los módulos básicos que tiene el firmware para controlar el ensayo. 

Con un plan a futuro de conectarse con un servidor flask, se plantea el usa de json para la comunicación, se requiera mantener el firmware monolítico dentro de lo posible. 

Para evitar lo complejo de la maquinas de estados a-sinconicras, el experimento no puede ser interrumpido mientras se esta realizando.

El formato del json es:


Funcionalidades del ensayo

A traves delo puerto serie y el formato json se configura la distancia donde se aplica la fuerza y la fuerza en cuestion 

{distance:'290'}      distance: Distancia en mm donde se aplica la fuerza(290mm).
{force:'5000'}        force: Fuerza a aplicar en g(5kg).



Al recibir del host el {cmd:'start'} el ensayo comienza , el logea los estado del mismo por el puerto serie.

// Lee por el puerto serie parametros de configuracion en formato json.
// {info:'all-params'}   Envia todos los parametros en formato json.
// {info:'version'}      Envia  la version del firmware.
// {info:'status'}       Devuelve el estatus del ensayo.
// {info:'reaction_one'}    Devuelve la reaction1 del ensayo.
// {info:'reaction_two'}    Devuelve la reaction2 del ensayo.
// {info:'flexion'}      Devuelve la flexion del ensayo.
// {info:'st_mode'}      Devuelve el modo del ensayo.


// {log_level:'0'}       log_level:0=desactivado,
// {log_level:'1'}                 1=mensajes.
// {log_level:'2'}                 2=info control estandar.
// {log_level:'3'}                 3=info control arduino plotter.

// {cmd:'start'}       Comienza el ensayo.

// {m1_fwd:'50'}           Mueve 50 mm el motor 1 hacia adelante.
// {m1_rwd:'4'}            Mueve 4 mm el motor 1 hacia atras.
// {step_m1_fwd:'200'}     Mueve 200 pasos el motor 1 hacia adelante.
// {step_m1_rwd:'200'}     Mueve 200 pasos el motor 1 hacia atras.


// {m2_up:'5'}              Mueve 5 mm el motor 1 hacia arriba.
// {m2_down:'4'}            Mueve 4 mm el motor 1 hacia abajo.
// {step_m2_up:'200'}       Mueve 200 pasos el motor 2 hacia arriba.
// {step_m2_down:'200'}     Mueve 200 pasos el motor 2 hacia abajo.

// {cdd:'start',data:{distance:'20',force:'306'}}  Se deja escrito para el cliente cdd=cmd


// {distance:'290'}      distance:     Distancia en mm donde se aplica la fuerza.
// {force:'12000'}       force:        Fuerza a aplicar en g.
// {reaction_one:'1'}    reaction_one  Fuerza de reaccion uno, en g.
// {reaction_two:'2'}    reaction_two  Fuerza de reaccion dos, en g.
// {flexion:'3'}         flexion       Flexion del ensayo, en mm.
// {st_test:'1'}         st_test       0 ensayo desactivado. 
//                       st_test       1 ensayo activado. 
// {st_mode:'0'}         st_mode       ST_MODE_TEST                    0  ensayo activado.
//                                     ST_MODE_TOF                     1  Modo de operacion TOF, muestra Tof sin promedio.
//                                     ST_MODE_TOF_AVERAGE             2  Modo de operacion TOF, muestra Tof con promedio.
//                                     ST_MODE_HOME_M2                 3  Va al home del motor 2. 
//                                     ST_MODE_CELL                    4  Lee las celdas de carga.

Maquina de estados 


La variable st_loop almacena el estado de la maquina


#define ST_LOOP_INIT                    0     // Inicializa el programa (carga la configuracion).
#define ST_LOOP_IDLE                    1     // Espera la recepcion por comando.
#define ST_LOOP_HOME_M1                 2     // Busca la referencia del Motor 1.
#define ST_LOOP_HOME_M2                 3     // Busca la referencia del Motor 1.
#define ST_LOOP_POINT_M1                4     // Se mueve m1 cantidada de pasos requeridos en mm.
#define ST_LOOP_GET_FLEXION_CERO        5     // Obtiene la referencia de la flexion
#define ST_LOOP_FORCE_M2                6     // Se mueve m2 hasta que encuentra la fuerza requerida en kilos.
#define ST_LOOP_GET_R1                  7     // Lee la celda de carga reaction1. 
#define ST_LOOP_GET_R2                  8     // Lee la celda de carga reaction2. 
#define ST_LOOP_GET_FORCE               9     // Lee la celda de la fuerza aplicada.
#define ST_LOOP_GET_FLEXION             10    // Lee la flexion. 
#define ST_LOOP_OFF_TEST                11    // Termino el ensayo.
#define ST_LOOP_TOF                     12    // Muestra el valor del tof
#define ST_LOOP_TOF_AVERAGE             13    // Muestra el valor del tof promedio 
#define ST_LOOP_MODE_HOME_M2            14    // Se mueve al home 2.
#define ST_LOOP_MODE_CELL               15    // Lee las celdas de carga.





