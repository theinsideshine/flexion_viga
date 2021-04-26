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

La alimentación es de 9v  para los motores y de 5V para el resto del equipo.

El core del equipo es un microcontrolador ATmega2560 de 256Kbytes de Flash, 8K de SRAM y 4K bytes de EEPROM de la empresa Microchip. Montado sobre la plataforma Arduino mega. Usa un cristal de 14.675Mhz para cubrir los baudarate estándar de RS232.

El control de los motores  se hace con un puente H integrado al driver DM8606 .
El uC lo controla por tres  pines STEP,DIR, ENAB  que le permiten regular la velocidad, modificar el sentido de giro, y habilitar y deshabilitar.

Controla dos finales de carrera. 

Posee 3 celdas de carga para medir fuerza. 2 de 10 kg y una de 50Kg , las tres poseen el HX771 ,encargado de acondicionar la señal de las galgas  internas a cada celda. Usa un conversor A/D dedicado de 24 bits, el cual devuelve un valor proporcional, a la deformación, por un protocolo serie 

Posee un sensor del tipo TOF que se encarga de medir al flexión  ,este sensor tiene un alcance de 20cm, y se controla por medio de IC2.

Para comunicarse con el host se utiliza un puerto serie USB que emula  RS232.

Posicion.

El motor 1 a través de un acople mecánico es solidario a una barra roscada M8, de 2m por paso, esta tiene acoplado al motor 2. La barra tiene un desplazamiento útil de 50 cm

 

Para establecer la posición del ensayo, se inicializa el punto de referencia cero, con el limite de carrera del motor1 y luego se mueve la cantidad de pasos necesarios para llegar a la distancia pedida.

Funcionalidades del ensayo

Al recibir del host el {cmd:'start'} el ensayo comienza , el logea los estado del mismo por el puerto serie.

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

Da soporte al debounce de los finales de carrera.

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

// Lee por el puerto serie parametros de configuracion en formato json.
// {info:'all-params'}   Envia todos los parametros en formato json.
// {info:'version'}      Envia  la version del firmware.

// {cmd:'start'}   Comienza el ensayo

// {log_level:'0'}       log_level:0=desactivado,
// {log_level:'1'}                 1=mensajes.
// {log_level:'2'}                 2=info control estandar.
// {log_level:'3'}                 3=info control arduino plotter.

// {distance:'500'}       distance:0 a 254       Distancia en cm donde se aplica la fuerza.
// {force:'11'}          force:0 a 65535          Fuerza a aplicar en Kg.
// {reaction_one:'1'}    reaction_one :0 a 65535  Fuerza de reaccion uno, en Kg.
// {reaction_two:'2'}    reaction_two :0 a 65535  Fuerza de reaccion dos, en Kg.
// {flexion:'3'}         flexion :0 a 65535       Flexion del ensayo, en mm.

// {st_test:'1'}         st_test : 0 ensayo desactivado. 
//                       st_test : 1 ensayo activado. 



Maquina de estados 


IDLE: espera que reciba desde el host {cmd:'start'}

ST_LOOP_INIT_M1: Si el final de carrera no se presiono mueve el motor1 1mm hacia atrás.

ST_LOOP_INIT_M2: Si el final de carrera no se presiono mueve el motor1 1mm hacia arriba.

ST_LOOP_POINT_M1: Mueve el motor1 la cantidad de mm requeridos  en la configuración.

ST_LOOP_FORCE_M2: Mueve el motor2 1mm hacia abajo , medida la fuerza aplicada, si llego a la fuerza requerida, se detiene. 

ST_LOOP_GET_R1: Lee la fuerza de reacción 1. 
ST_LOOP_GET_R2: Lee la fuerza de reacción 2. 
ST_LOOP_GET_R1: Lee la flexion de la viga. 

ST_LOOP_OFF_TEST: Termina el ensayo 



La variable st_loop almacena el estado de la maquina

#define ST_LOOP_INIT                    0     // Inicializa el programa (carga la configuración).
#define ST_LOOP_IDLE                    1     // Espera la recepción por comando.
#define ST_LOOP_INIT_M1                 2     // Busca la referencia del Motor 1.
#define ST_LOOP_INIT_M2                 3     // Busca la referencia del Motor 1.
#define ST_LOOP_POINT_M1                4     // Se mueve m1 cantidad de pasos requeridos en mm.
#define ST_LOOP_FORCE_M2                5     // Se mueve m2 hasta que encuentra la fuerza requerida en kilos.
#define ST_LOOP_GET_R1                  6     // Lee la celda de carga reaction1. 
#define ST_LOOP_GET_R2                  7     // Lee la celda de carga reaction2. 
#define ST_LOOP_GET_FLEXION             8     // Lee la celda de carga reaction2. 
#define ST_LOOP_OFF_TEST                9     // Termino el ensayo.
