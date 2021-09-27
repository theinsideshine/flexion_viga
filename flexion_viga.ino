/**
   File:   The system is composed of two stepper motors, motor 1 is responsible for moving motor 2,
          to the distance where the test force is applied, and motor 2 is the one that applies said force.
          Motor 1 is open-loop controlled.
          The motor is closed-loop controlled with a load cell, which we call cell_force.
          Each motor has a limit switch to be able to take a movement reference.
          At the ends of the beam we have two load cells that measure the reaction forces, we call them cell_reaction1, cell_reaction2.
          In the middle of the beam and below we have a TOF sensor, the VL6180x, which is responsible for measuring the bending of the beam at the center point.

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
#include "tof.h"
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

Clog    Log;
CConfig Config;
CButton Button;
CMotor   Motor;
CCell    Cell;
CTof     Tof;
CLed     Led;


/*
 * Mueve el motor 2 a la posicion de home, luego
 *  avanza dos vueltas para eliminar el error de contraccion. 
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
 *  avanza dos vueltas para eliminar el error de contraccion. 
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
  Log.msg( F("asi se puede ver valor crudo. {st_mode:'4'}") );

#endif

#ifdef BUTTON_PRESENT
  Button.init();
  Log.msg( F("Button init") );
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

#ifdef TOF_PRESENT

  if (Tof.init()) {
    Log.msg( F("Tof presente") );
  } else {
    Serial.println("Error al buscar tof");
    while (1);
  }
#endif //TOF_PRESENT


  Log.msg( F("Sistema inicializado") );

}

 
/*
 *Loop de control del Ensayo viga simplemente apoyada 
 */


void loop()
{
  static CTimer   Timer; // NO se usa
  static uint8_t  st_loop = ST_LOOP_INIT;
  static float peso = 0 ;

  /*
     TODO:cuando el ensayo es en ejecucion no debe escuchar comandos

  */
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
    
      if (Config.get_st_test() == true ) {// Espera que se comienzo al ensayo.
        
        st_loop = ST_LOOP_HOME_M2; 

      }else if (Config.get_st_mode() == ST_MODE_TOF ) { // Espera  modo calibracion TOF sin promedio.
        st_loop = ST_LOOP_TOF ;
      }else if (Config.get_st_mode() == ST_MODE_TOF_AVERAGE ) { // Espera que modo calibracion TOF con promedio.
        st_loop = ST_LOOP_TOF_AVERAGE ;
      }else if (Config.get_st_mode() == ST_MODE_HOME_M2 ) { // Espera que modo home2.
        st_loop =  ST_LOOP_MODE_HOME_M2 ;
      }else if (Config.get_st_mode() == ST_MODE_CELL ) { // Espera que modo celdas de cargas.
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
      st_loop = ST_LOOP_GET_FLEXION_CERO;

      break;

    case ST_LOOP_GET_FLEXION_CERO:   // Reserva este estado hasta definir la referencia de la medicion de la flexion.

      Tof.set_tof_cero();            // Toma la referencia definida en CERO_FLEXION - tof.h.
      st_loop = ST_LOOP_POINT_M1;   
     
      
      break;

    //Mueve el motor1 en direccion forward ,la distance en mm de la configuracion.
    case ST_LOOP_POINT_M1:


      Log.msg( F("Moviendo el motor 1 cantidad de milimitros "));


      Motor.fwd_m1(Motor.m1_convertion_distance(Config.get_distance()));       //Convierte la distancia real de la viga a la distancia util del motor
      st_loop = ST_LOOP_FORCE_M2;
      home_m2(M2_HOME_OFFSET);   // Se mueve el offset configurado.
      Motor.rst_counter_m2();   // Resetea el contador del motor.
      Led.on(); //prende led apoyar peso
      delay(1000); // Espera para pasar de estado
      Log.msg( F("Moviendo el motor 2 haste leer la fuerza configurada "));
#ifdef CELL_DEBUG 
      Serial.println("Fuerza    pasos");
#endif
      break;

    //Mueve el motor 2 en direccion down ,hasta que se aplique la fuerza en gramos de la configuracion.
    case ST_LOOP_FORCE_M2:      
      
      Cell.read_cell_force();
      if ( Cell.is_cell_load()){ // maneja el estado de movimiento sin carga y con carga

          Motor.rst_counter_m2();   // Resetea el contador del motor.
        
      }
      if ( Cell.is_force(Config.get_force())) { // Llego a la fuerza configurada?
#ifdef CELL_DEBUG 

        Serial.print(" ");                           // Imprime ultimo resultado para completar la tabla: fuerza pasos,
        Serial.println(Motor.get_counter_m2());
        Serial.print("Pasos m2= ");                  // Muestra la cantidad de pasos que se movio desde home,medicion exploratoria.
        Serial.println(Motor.get_counter_m2());
      
#endif
        Log.msg( F("Force:Ok "));
        Led.off(); //apaga led
        st_loop = ST_LOOP_GET_R1 ;
        Cell.reset_cell_load();                              // Resetea el estado de movimiento con carga.
        delay(CELL_TIME_READ_FORCE); // Espera para pasar de estado
      } else {
#ifdef CELL_DEBUG       
        Serial.print(" ");
        Serial.println(Motor.get_counter_m2());   
#endif               
        Motor.step_m2_down(M2_DOWN_FORCE_STEP);      //  Mueve cantidad de pasos hacia abajo al motor 2
        Motor.inc_counter_m2(M2_DOWN_FORCE_STEP);    // Incrementa el contador de pasos.
      }

      break;

    //Lee la fuerza de reaccion 1 y la guarda en la configuracion.
    case ST_LOOP_GET_R1:
      Log.msg( F("Leyendo reaccion 1"));
      Config.set_reaction1(Cell.get_cell_reaction1());
      st_loop = ST_LOOP_GET_R2;
      delay(1000); // Espera para pasar de estado.

      break;

    //Lee la fuerza de reaccion 2 y la guarda en la configuracion.
    case ST_LOOP_GET_R2:
      Log.msg( F("Leyendo reaccion 2"));
      Config.set_reaction2(Cell.get_cell_reaction2());
      st_loop = ST_LOOP_GET_FORCE;
      delay(1000); // Espera para pasar de estado

      break;

    // Guarda en la eprrom la fuerza aplicada.
    case ST_LOOP_GET_FORCE:

      Cell.read_cell_force();
      Config.set_force(Cell.get_cell_force());
      st_loop = ST_LOOP_GET_FLEXION;

      break;

    // Lee la distancia de flexion y la guarda en la configuracion.
    case ST_LOOP_GET_FLEXION:

#ifdef TOF_PRESENT


      Log.msg( F("Lectura del tof"));
      Tof.get_tof_average();                       // Toma un promedio y lo guarda en la distancia 1.
      Config.set_tof_flexion(Tof.get_tof_flexion());   // Devuelve la diferencia entra la distancia 0 y 1. y lo guarda en la eeprom.

#else
      Log.msg( F("tof ausente."));


#endif

      Config.set_step_flexion(Motor.calculate_step_flexion()); // Guarda la flexion calculada por los pasos. 
      st_loop = ST_LOOP_OFF_TEST;
      break;
     
     // Termina el ensayo.
    case ST_LOOP_OFF_TEST:
    
    
      end_test();
      st_loop = ST_LOOP_IDLE;
      break;

    // Modo calibacion del tof sin promedio.  

    case ST_LOOP_TOF:

#ifdef TOF_PRESENT

      if (Tof.read_status())
      {
        Log.msg( F("TOF.= %d"),Tof.get_tof());    // Muestra la lectura del tof
      }
      else{
        Tof.print_status();   // Imprime el error del tof 
       }
       delay(1000); // Espera para tomar la proxima muestra.
#else 
      
      Log.msg( F("Tof ausente.")); 
#endif 
      
      if (Config.get_st_mode() == ST_MODE_TEST ) { // Espera que modo calibracion sin promedio TOF sea terminado por el usuario.
        st_loop = ST_LOOP_IDLE;
      }
      
      break;

      // Modo calibacion del tof con promedio.
    
      case ST_LOOP_TOF_AVERAGE:

#ifdef TOF_PRESENT

        Log.msg( F("Promedio TOF.= %d"),Tof.get_tof_average());    // Muestra la lectura promedio del tof
      
#else 
      
      Log.msg( F("Tof promedio ausente.")); 
#endif 
      
      if (Config.get_st_mode() == ST_MODE_TEST ) { // Espera que modo calibracion con promedio TOF sea terminado por el usuario
        st_loop = ST_LOOP_IDLE;
      }
      delay(1000); // Espera para tomar el proximo promedio.
      
      break;

      // Modo home2
      case ST_LOOP_MODE_HOME_M2:

      home_m2(M2_HOME_OFFSET);
      delay(1000); // Espera para pasar de estado.
      Config.set_st_mode( ST_MODE_TEST );
      st_loop = ST_LOOP_IDLE;

      break;
      
      // Modo Celdas
      case ST_LOOP_MODE_CELL:
#ifdef CELL_PRESENT
      if (Config.get_st_mode() == ST_MODE_TEST ) { // Espera que modo celdas sea terminado por el usuario
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
#else 
      
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
