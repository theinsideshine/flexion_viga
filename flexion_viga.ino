/**
   File:  
   
   Resumen
            Se propone automatizar  un banco de ensayo del tipo viga simplemente apoyada. 
            En este se aplica una carga puntual en una posici�n de la viga y se miden sus reacciones y flexión en  la posici�n. 


             Se proponen dos actuadores, un control de posición a  lazo abierto, y un control de fuerza a lazo cerrado.
            Para el control de posiciòn se contar�n los pulso del motor 1, y para el control de fuerza se propone aplicar 
            una cantidad fija de pasos con el motor 2 y medir la carga aplicada con un sensor del tipo celda de carga.,
            las reacciones se medirán con dos celdas de cargas. Para medir la flexión se contarón los pasos del motor 2.

    Antecedentes
            
            La automatización se hizo sobre el  documento "Proyecto Final 2019 - Banco de prueba - Viga sometida a carga puntual R3"
            En base a las experiencias anterior de laboratorio remoto se considera
            errores relativos m�ximos para el ensayo es:
                                                      Reacciones 5%
                                                      Flexión       10%
Introducción

Las partes principales son:

            Sistema Mecánico
            
            El sistema está compuesto  de dos motores paso a paso, el motor 1 se encarga de  mover el motor 2,
            a la distancia  donde se aplica la carga del ensayo, y el motor 2 es el que aplica dicha carga.
            El motor 1  está controlado a lazo abierto.
           
            Cada motor tiene un final de carrera para poder tomar una referencia de movimiento.
            En los extremos de la viga tenemos dos celdas de cargas que miden las fuerzas de reacci�n, estas las llamamos   cell_reaction 1, cell_reaction 2.
            El motor 2 está controlado a lazo cerrado con una celda de carga, que llamamos cell_force= cell_reaction 1 + cell_reaction 2
 
            Placa electrónica
            La alimentaci�n es de 24v  para los motores y de 5V para el resto del equipo.
            El core del equipo es un microcontrolador ATmega2560 de 256 Kbytes de Flash, 8K de SRAM y 4K bytes de EEPROM 
            de la empresa Microchip. Montado sobre la plataforma Arduino Mega. Usa un cristal de 14.675Mhz para cubrir los baudrate estándar de RS232.
            El control de los motores  se hace con driver Dm542E
            El microcontrolador lo controla por dos pines PUL,DIR,  que le permiten regular la velocidad, modificar el sentido de giro y controlar tres finales de carrera.
            Posee 3 celdas de carga para medir la fuerza. 2 de 10 kg y una de 50 Kg , las tres poseen el HX771 ,
            encargado de acondicionar la señal de las galgas  internas a cada celda. Usa un conversor A/D dedicado de 24 bits, 
            el cual devuelve un valor proporcional, a la deformación, por un protocolo serie 
            Para comunicarse con el host se utiliza un puerto serie USB que emula  RS232.
            
            Posición.
            El motor 1 a través de un acople mecnico es solidario a una barra roscada M8, de 2 mm por vuelta,
            esta tiene acoplado al motor 2. La barra tiene un desplazamiento ´útil de 30 cm
            El motor 2 tiene tornillo de paso 1mm por vuelta
            
            Firmware
            Para poder cumplir con la buenas prácticas de la programación y asíko darle solidez,escalabilidad y mantenimiento simple al proyecto,
            se desarrolló la siguiente arquitectura en C++, compilado con el ide arduino.

            Donde el usuario se comunica con la cpu mediante un sistema de host que comparte la memoria con la cpu, 
            la cpu está escuchando los cambios en la memoria, y actúa según estos. Con esto se logra la abstracción de todas las partes del sistema en clases ,
            que solo son consumidas desde la cpu
            

   - Compiler:           Arduino 1.8.13
   - Supported devices:  Mega

   \author               MV: luis.villacorta@alumnos.udemm.edu.ar
                         LC: leandro.cintioli@alumnos.udemm.edu.ar
                         PT: pablo.tavolaro@alumnos.udemm.edu.ar

   Date:  17-04-2021

        Universidad de la Marina Mercante.
*/




#include "log.h"
#include "cfg.h"
#include "timer.h"
#include "button.h"
#include "motor.h"
#include "cell.h"
#include "led.h"
#include "precompilation.h"


/*
 * Estado del loop principal de ejecucion
 */

#define ST_LOOP_INIT                    0     // Inicializa el programa (carga la configuracion).
#define ST_LOOP_IDLE                    1     // Espera la recepcion por comando.
#define ST_LOOP_HOME_M1                 2     // Busca la referencia del Motor 1.
#define ST_LOOP_HOME_M2                 3     // Busca la referencia del Motor 1.
#define ST_LOOP_POINT_M1                4     // Se mueve m1 cantidada de pasos requeridos en mm.

#define ST_LOOP_FORCE_M2                5     // Se mueve m2 hasta que encuentra la fuerza requerida en kilos.
#define ST_LOOP_GET_R1                  6     // Lee la celda de carga reaction1. 
#define ST_LOOP_GET_R2                  7     // Lee la celda de carga reaction2. 
#define ST_LOOP_GET_FLEXION             8     // Calcula la flexion y la guarda en la configuracion
#define ST_LOOP_OFF_TEST                9    // Termino el ensayo.
#define ST_LOOP_MODE_HOME_M2            10    // Se mueve al home 2.
#define ST_LOOP_MODE_CELL               11    // Lee las celdas de carga.

/*
 * Clases del sistema
 */

Clog    Log;
CConfig Config;
CButton Button;
CMotor   Motor;
CCell    Cell;
CLed     Led;

/*
 * Mueve el motor 2 a la posicion de home.
 *.parametros: offset cantidad de pasos que avanza luego del home. 
 */

void home_m2(  uint32_t offset ) {

  if ( Button.is_button_m2_low() ) {
    Log.msg( F("M2 esta en home") );
    
  } else {
    Motor.pwm_on_m2();
    Log.msg( F("Buscando el home del M2") );
   // Espera que llegue al home
     while (  !Button.is_button_m2_low() );
     Motor.pwm_off_m2();
     Log.msg( F("M2 se llevo a home") );
     }
     
    if (offset != M2_HOME_OFFSET_CERO){
      Motor.m2_offset_home(offset);
      Log.msg( F("M2 se movio hasta el offset") );
  }
}

/*
 * Mueve el motor 1 a la posicion de home, luego
 *  
 */
 
void home_m1( void ) {

  if ( Button.is_button_m1_low() ) {
    Log.msg( F("M1 esta en home") );
    return true;
  } else {
    Motor.pwm_on_m1();
  }
  Log.msg( F("Buscando el home del M1") );
  // Espera que llegue al home
  while (  !Button.is_button_m1_low() );
  Motor.pwm_off_m1();
  Log.msg( F("M1 se llevo a home") );

}

/*
 * Configura el final de ensayo
 * vuelve el st_test = 0 y envia el valor al servidor.
 */

void end_test( void ) {

  Log.msg( F("Buscando HOMEs!!"));             
  home_m2(M2_HOME_OFFSET_CERO);                // Busca el home 2.
  delay(1000);                                // Absorve el deBounce.
  home_m1();                                  // Busca el home 1.
  Led.n_blink(3, 1000);                       // 2 blinks cada 1000 ms;
  Log.msg( F("ENSAYO TERMINADO, SUERTE!!!"));
  Config.set_st_test( false );
  Config.send_test_finish(); // Informa al servidor que termino el ensayo.
}

/*
 *  Inicializa los dispositivos del ensayo segun opciones de precompilacion.
 */

void setup()
{
  Log.init( Config.get_log_level() );
  Serial.println("Init Serial");
  /*
      Para activar la visualisacion  enviar por serie {log_level:'1'}
  */

  Log.msg( F("Ensayo viga simplemente apoyada - %s"), FIRMWARE_VERSION );
  Log.msg( F("UDEMM - 2021") );

#ifdef TEST_PROTOTIPE
  Log.msg( F("Modo: Ensayo prototipo") );
#else
  Log.msg( F("Modo: Ensayo laboratorio remoto ") );
#endif

#ifdef CALIBRATION_CELL_FORCE

  Log.msg( F("CALIBRACION DE CELDA DE FUERZA") );
  Log.msg( F("No se inicializa la escala de la celda") );
  Log.msg( F("asi se puede ver valor crudo. {st_mode:'2'}") );

#endif

#ifdef BUTTON_PRESENT
  Button.init();
  Log.msg( F("Damper init") );
#endif

#ifdef MOTOR_PRESENT
  Motor.init();
  Log.msg( F("Motor init") );
#endif // MOTOR_PRESENT

#ifdef LED_PRESENT
  Led.init();
  Log.msg( F("Led init") );
  Led.n_blink(2, 1000); // 2 blinks cada 1000 ms
#endif // LED_PRESENT   

#ifdef CELL_PRESENT

  Log.msg( F("Aguarde 3 segundos antes de comenzar la busqueda del home del M2") );
  delay(3000);  // Espera 3 segundo para evitar el movimiento apenas se carga el firmware.
  home_m2(M2_HOME_OFFSET_CERO);
  Cell.init();
  Log.msg( F("Cell init") );

#endif //CELL_PRESENT

  Log.msg( F("Sistema inicializado") );

}
 
/*
 *Loop de control del Ensayo viga simplemente apoyada 
 */

void loop()
{
  static uint8_t  st_loop = ST_LOOP_INIT;   
  
  // Verifica si el host envio un JSON con parametros a procesar.
  Config.host_cmd();
  
  // Actualiza el nivel de log para detener en tiempo real el envio de parametros.
  Log.set_level( Config.get_log_level() );
  
  switch ( st_loop ) {

    case ST_LOOP_INIT:

      st_loop = ST_LOOP_IDLE ;

    break;

    // Espera "eventos" de ejecucion
    case ST_LOOP_IDLE:
    
      if (Config.get_st_test() == true ) {                   // Espera que se comienzo al ensayo.        
        st_loop = ST_LOOP_HOME_M2; 
      }else if (Config.get_st_mode() == ST_MODE_HOME_M2 ) { // Espera el modo home2.
        st_loop =  ST_LOOP_MODE_HOME_M2 ;
      }else if (Config.get_st_mode() == ST_MODE_CELL ) {    // Espera el modo celdas de cargas.
        st_loop =  ST_LOOP_MODE_CELL ;
      }
      
    break;

    //Mueve en direccion up el motor2 hasta que se presiones el final de carrera m2.
    case ST_LOOP_HOME_M2:

      home_m2(M2_HOME_OFFSET_CERO);         
      delay(1000); // Espera para pasar de estado.
      st_loop = ST_LOOP_HOME_M1;

    break;

    //Mueve en direccion rewind el motor1 hasta que se presiones el final de carrera m1.
    case ST_LOOP_HOME_M1:
    
      home_m1();
      delay(1000); // Espera para pasar de estado.
      st_loop = ST_LOOP_POINT_M1;
      
    break;

    //Mueve el motor1 en direccion forward ,la distance en mm de la configuracion.
    case ST_LOOP_POINT_M1:

      Log.msg( F("Moviendo el motor 1 cantidad de milimitros "));
      
      Motor.fwd_m1(Motor.m1_convertion_distance(Config.get_distance()));      // Convierte la distancia real de la viga a la distancia util del motor.
           
      home_m2(M2_HOME_OFFSET);                                                // Se mueve el offset configurado.
      Motor.rst_counter_m2();                                                 // Resetea el contador del motor.
      Led.on();                                                               // Prende led apoyar peso.
      delay(1000);                                                            // Espera para pasar de estado.
      
      Log.msg( F("Moviendo el motor 2 haste leer la fuerza configurada ")); 
      
#ifdef CELL_DEBUG 
      Serial.println("Fuerza    pasos");
#endif

      
      st_loop = ST_LOOP_FORCE_M2;
      
    break;

    //Mueve el motor 2 en direccion down ,hasta que se aplique la fuerza en gramos de la configuracion.
    case ST_LOOP_FORCE_M2: 
               
      Cell.read_cell_force();                                                 // Lee la celda de fuerza.
      
      if ( Cell.is_cell_load()){                                              // Maneja el estado de movimiento sin carga y con carga.         
          Motor.rst_counter_m2();                                             // Resetea el contador del motor.        
      }
      
      if ( Cell.is_force(Config.get_force())) {                               // Llego a la fuerza configurada?
        
#ifdef CELL_DEBUG          
        Serial.print("Pasos m2= ");                                           // Muestra la cantidad de pasos que se movio desde home, medicion exploratoria.
        Serial.println(Motor.get_counter_m2());      
#endif
        
        /*
         * Grabar cantidad de pasos 
         */

        Config.set_step_cal(Motor.get_counter_m2());
        Log.msg( F("Force:Ok "));
        
        Led.off();                                                            // Apaga led.        
        Config.set_force(Cell.get_cell_force());                              // Carga la fuerza leida.        
        Cell.reset_cell_load();                                               // Resetea el estado de movimiento con carga.        
        delay(CELL_TIME_READ_FORCE);                                          // Espera para pasar de estado.
        st_loop = ST_LOOP_GET_R1 ;
        
      } else {
#ifdef CELL_DEBUG  
        Serial.print(Cell.get_cell_force(),1);     
        Serial.print(" ");
        Serial.println(Motor.get_counter_m2());   
#endif               
        Motor.step_m2_down(M2_DOWN_FORCE_STEP);                               //  Mueve cantidad de pasos hacia abajo al motor 2.
        Motor.inc_counter_m2(M2_DOWN_FORCE_STEP);                             // Incrementa el contador de pasos.
      }
        
        
    break;

    //Lee la fuerza de reaccion 1 y la guarda en la configuracion.
    case ST_LOOP_GET_R1:
    
      Log.msg( F("Leyendo reaccion 1"));
      Config.set_reaction1(Cell.get_cell_reaction1());
      st_loop = ST_LOOP_GET_R2;
      delay(CELL_TIME_READ_FORCE);                                              // Espera para pasar de estado.
            
     break;

    //Lee la fuerza de reaccion 2 y la guarda en la configuracion.
    case ST_LOOP_GET_R2:
    
      Log.msg( F("Leyendo reaccion 2"));
      Config.set_reaction2(Cell.get_cell_reaction2());
      st_loop = ST_LOOP_GET_FLEXION;
      delay(1000); // Espera para pasar de estado

      break;
      
      // Calcula la flexion y la guarda en la configuracion.
    case ST_LOOP_GET_FLEXION:

      /*
       * Mandar constante extraida de config
       */
       
      Config.set_flexion(Motor.calculate_flexion(Config.get_step_k()));           // Guarda la flexion calculada por los pasos. 
      st_loop = ST_LOOP_OFF_TEST;
      
      break;
      
     // Termina el ensayo.
    case ST_LOOP_OFF_TEST:    
    
      end_test();
      st_loop = ST_LOOP_IDLE;
      
      break;  
      
      // Modo home2
      case ST_LOOP_MODE_HOME_M2:

      home_m2(M2_HOME_OFFSET);
      delay(1000);                                                               // Espera para pasar de estado.
      Config.set_st_mode( ST_MODE_TEST );
      st_loop = ST_LOOP_IDLE;

      break;
      
      // Modo Celdas
      case ST_LOOP_MODE_CELL:
      
#ifdef CELL_PRESENT
      if (Config.get_st_mode() == ST_MODE_TEST ) {                               // Espera que modo celdas sea terminado por el usuario.
        st_loop = ST_LOOP_IDLE;
      }
      Serial.print("Reaccion1: ");
      Serial.println(Cell.get_cell_reaction1(),1);
      Serial.print("Reaccion2: ");
      Serial.println(Cell.get_cell_reaction2(),1);
#ifdef CALIBRATION_CELL_FORCE
      Serial.print("Fuerza cruda: ");
#else       
      Serial.print("Fuerza aplicada: ");
#endif //CALIBRATION_CELL_FORCE
      Cell.read_cell_force();
      Serial.println(Cell.get_cell_force(),1);
      delay(1000); // Espera para volvel.
#else // CALIBRATION_CELL_FORCE
      
      Log.msg( F("Celdas ausentes.")); 
#endif // CELL_PRESENT
    
      break;

      default:
      st_loop = ST_LOOP_INIT;

  }
#ifdef ST_DEBUG
  Log.msg( F("ST_LOOP= %d"), st_loop );
#endif //ST_DEBUG

}
