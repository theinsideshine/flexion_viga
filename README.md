# flexion_viga

Introducción 

El sistema consistió en un aparato para medir reacciones de apoyos y flexión por deformación elástica causados por una carga que se aplica en una viga simplemente apoyada (isostática). Dichos parámetros se obtienen por medio de sensores de carga para las reacciones y  sensor o medidor  de distancia para la flexión..
El Banco de prueba de laboratorio podrá ser operado a distancia (vía web), el sistema permitirá modificar la posición e intensidad de la carga aplicada en cada ensayo. 
La plataforma de la universidad proporcionará al individuo los valores que le permitan llegar a desarrollar los diagramas característicos de viga simplemente apoyada.
Puesto que dichos ensayos pueden llevarse a cabo de manera remota y para favorecer la experiencia visual del operador, se transmitirá el ensayo en vivo a través de una cámara situada junto al aparato.
.



Objetivos
El objetivo del documento es describir las funcionalidades y componentes que tiene el
firmware.
Para una mejor comprensión del firmware se describe el equipo mecánico que hay que
controlar y el hardware de la placa electrónica.

Sistema Mecánico
El sistema está compuesto  de dos motores paso a paso, el motor “1” es el encargado de determinar la distancia  donde se aplica la fuerza a posteriori del ensayo, y el motor “2” es el que aplica dicha fuerza.
El motor 1  está controlado a lazo abierto.
Cada motor tiene un final de carrera para poder tomar una referencia de movimiento.
En los extremos de la viga tenemos dos celdas de cargas que miden las fuerzas de reacción, estas las llamamos   cell_reaction 1, cell_reaction 2.
El motor 2 está controlado a lazo cerrado por la suma de  cell_reaction 1 y cell_reaction 2.

 
Módulos de electrónica
La alimentación es de 24v  para los motores y de 5V para el resto del equipo.

Microcontrolador ATmega2560 de 256 Kbytes de Flash, 8K de SRAM y 4K bytes de EEPROM de la empresa Microchip. Montado sobre la plataforma Arduino Mega. Usa un cristal de 14.675Mhz para cubrir los baudrate estándar de RS232.

RaspBerry pi 3: Se usa como servidor web 

2 (dos) unidades  Driver Dm542E 


El microcontrolador realiza el control por dos pines PUL,DIR,  que le permiten regular la velocidad, modificar el sentido de giro y controlar dos finales de carrera. 


Posee 2 celdas de carga de 20 Kg para medir la fuerza. , las dos poseen el HX771, encargado de acondicionar la señal de las galgas internas a cada celda. Usa un conversor A/D dedicado de 24 bits, el cual devuelve un valor proporcional,  por un protocolo serie.

Para comunicarse con el host se utiliza un puerto serie USB que emula  RS232.

Posición.
El motor 1 a través de un acople mecánico es solidario a una barra roscada M8, de 2 mm por paso, esta tiene acoplado al motor 2. La barra tiene un desplazamiento útil desde los 20 cm de posición del cursor a los 50 cm que es la mitad de la longitud de la viga. El paso del motor 2 es de 1 mm  por vuelta

 Sobre la viga el motor 2  tiene un desplazamiento útil de 20.6cm 50 cm
 

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





