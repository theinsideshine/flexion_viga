/**
 * File:   The system is composed of two stepper motors, motor 1 is responsible for moving motor 2, 
 *        to the distance where the test force is applied, and motor 2 is the one that applies said force.
 *        Motor 1 is open-loop controlled.
 *        The motor is closed-loop controlled with a load cell, which we call cell_force.
 *        Each motor has a limit switch to be able to take a movement reference.
 *        At the ends of the beam we have two load cells that measure the reaction forces, we call them cell_reaction1, cell_reaction2.
 *        In the middle of the beam and below we have a TOF sensor, the VL6180x, which is responsible for measuring the bending of the beam at the center point.
 *
 * - Compiler:           Arduino 1.8.13
 * - Supported devices:  Mega
 *
 * \author               MV: luis.villacorta@alumnos.udemm.edu.ar
 *                       LC: leandro.cintioli@alumnos.udemm.edu.ar
 *                       PT: pablo.tavolaro@alumnos.udemm.edu.ar
 *
 * Date:  17-04-2021
 *
 *      Universidad de la Marina Mercante.
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


#define ST_LOOP_INIT                    0     // Inicializa el programa (carga la configuracion).
#define ST_LOOP_IDLE                    1     // Espera la recepcion por comando.
#define ST_LOOP_HOME_M1                 2     // Busca la referencia del Motor 1.
#define ST_LOOP_HOME_M2                 3     // Busca la referencia del Motor 1.
#define ST_LOOP_POINT_M1                4     // Se mueve m1 cantidada de pasos requeridos en mm.
#define ST_LOOP_FORCE_M2                5     // Se mueve m2 hasta que encuentra la fuerza requerida en kilos.
#define ST_LOOP_GET_R1                  6     // Lee la celda de carga reaction1. 
#define ST_LOOP_GET_R2                  7     // Lee la celda de carga reaction2. 
#define ST_LOOP_GET_FLEXION             8     // Lee la celda de carga reaction2. 
#define ST_LOOP_OFF_TEST                9     // Termino el ensayo.


Clog    Log;
CConfig Config;
CButton Button;
CMotor   Motor;
CCell    Cell;
CTof     Tof;
CLed     Led;

// Se asegura que el motor 2  se mueva al home.
// return true cuando termina. No se usa aun el valor de retorno

bool home_m2( void ){

  if ( Button.is_button_m2_low() ){
        Log.msg( F("M2 esta en home") );
        return true;
  }else{
    Motor.pwm_on_m2();    
  }
  Log.msg( F("Buscando el home del M2") );
  // Espera que llegue al home  
  while (  !Button.is_button_m2_low() );
  Motor.pwm_off_m2();  
  Log.msg( F("M2 se llevo a home") );
  
  return true;
  
 }

// Se asegura que el motor 1  se mueva al home.
// return true cuando termina. No se usa aun el valor de retorno

bool home_m1( void ){

  if ( Button.is_button_m1_low() ){
        Log.msg( F("M1 esta en home") );
        return true;
  }else{
    Motor.pwm_on_m1();    
  }
  Log.msg( F("Buscando el home del M1") );
  // Espera que llegue al home  
  while (  !Button.is_button_m1_low() );
  Motor.pwm_off_m1();  
  Log.msg( F("M1 se llevo a home") );
  
  return true;
  
 }
 



// Inicializa los dispositivos del ensayo segun opciones de precompilacion. 
void setup()
{
   Log.init( Config.get_log_level() );  
   Serial.println("Init Serial"); 
 /*
  *  Para activar la visualisacion  enviar por serie {log_level:'1'}    
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
   Led.n_blink(2,1000); // 2 blinks cada 1000 ms
#endif // LED_PRESENT   
   
#ifdef CELL_PRESENT   

   Log.msg( F("Aguarde 3 segundos antes de comenzar la busqueda del home del M2") ); 
   delay(3000);  // Espera 3 segundo para evitar el movimiento apenas se carga el firmware.
   home_m2(); 
   Cell.init();   
   Log.msg( F("Cell init") );
   
#endif //CELL_PRESENT

#ifdef TOF_PRESENT  
  
   if (Tof.init()){
      Log.msg( F("Tof presente") );
   }else {
        Serial.println("Error al buscar tof"); 
         while (1);
        }
#endif //TOF_PRESENT

 
  Log.msg( F("Sistema inicializado") );
 
 }


// Loop de control del Ensayo viga simplemente apoyada
void loop()
{
static CTimer   Timer;
static uint8_t  st_loop = ST_LOOP_INIT;  
static float peso = 0 ;



       
    /*
     * TODO:cuando el ensayo es en ejecucion no debe escuchar comandos 
     * 
     */
     // Verifica si el host envio un JSON con parametros a procesar.
           Config.host_cmd();
     // Actualiza el nivel de log para detener en tiempo real el envio de parametros.
         Log.set_level( Config.get_log_level() ); 
              
    
    
    switch( st_loop ) {
       
        case ST_LOOP_INIT:
            st_loop = ST_LOOP_IDLE ; 
        break;   

       
         // Espera que se comienzo al ensayo.
         
        
        case ST_LOOP_IDLE:
       
        
        if (Config.get_st_test()== true ){

           // Espera que se comienzo al ensayo.   
             
#ifdef CALIBRATION_CELL_FORCE

       st_loop = ST_LOOP_GET_R1; //add macro precompilation .
#else 
       st_loop = ST_LOOP_HOME_M2;       
   
#endif  // CALIBRATION_CELL_FORCE       
          
          
        }
        break; 

          //Mueve en direccion up el motor2 hasta que se presiones el final de carrera m2.
            
         case ST_LOOP_HOME_M2:            
            
            home_m2();               
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
              Motor.fwd_m1(Config.get_distance());                        
              st_loop = ST_LOOP_FORCE_M2;
              Led.on(); //prende led apoyar peso
              delay(1000); // Espera para pasar de estado 
        break;

          //Mueve el motor 2 en direccion down ,hasta que se aplique la fuerza en gramos de la configuracion.
         case ST_LOOP_FORCE_M2:
         
              Log.msg( F("Moviendo el motor 2 haste leer la fuerza configurada "));             
              Cell.read_cell_force();  
              if ( Cell.is_force(Config.get_force())) {
                                
                 Log.msg( F("Force:Ok "));
                 Led.off(); //apaga led 
                 st_loop = ST_LOOP_GET_R1 ;
                 delay(1000); // Espera para pasar de estado                  
              }else {
                
                Motor.step_m2_down(M2_DOWN_FORCE_STEP);  //  Mueve cantidad de pasos hacia abajo al motor 2
              }
                              
        break;

          //Lee la fuerza de reaccion 1 y la guarda en la configuracion.
        case ST_LOOP_GET_R1:
#ifndef CALIBRATION_CELL_FORCE

                Log.msg( F("Lectura de fuerza de reaccion 1 en 3 segundos ponga el peso"));
                
#endif //CALIBRATION_CELL_FORCE
                
                 delay(3000); //Tiempo para poner otro peso (DEBUG).
                
                Config.set_reaction1(Cell.read_cell_reaction1());
                      
               st_loop = ST_LOOP_GET_R2;
               delay(1000); // Espera para pasar de estado.

        break;

        //Lee la fuerza de reaccion 2 y la guarda en la configuracion.
        case ST_LOOP_GET_R2:

#ifndef CALIBRATION_CELL_FORCE

                Log.msg( F("Lectura de fuerza de reaccion 2 en 3 segundos ponga el peso"));
                
#endif //CALIBRATION_CELL_FORCE
                
                delay(3000); //tiempo para poner otro peso(DEBUG).                
                Config.set_reaction2(Cell.read_cell_reaction2());
                
#ifdef CALIBRATION_CELL_FORCE

               Cell.read_cell_force();
               st_loop = ST_LOOP_OFF_TEST;
#else 
               st_loop = ST_LOOP_GET_FLEXION;
               delay(1000); // Espera para pasar de estado 
  
   
#endif  // CALIBRATION_CELL_FORCE
                
        break;

        //Lee la distancia de flexion y la guarda en la configuracion.
        case ST_LOOP_GET_FLEXION:

#ifdef TOF_PRESENT  
        
              Log.msg( F("Lectura del tof"));              
              if (Tof.read_status()){
                 Config.set_flexion(Tof.read_tof());
                 st_loop = ST_LOOP_OFF_TEST;
                 delay(1000); // Espera para pasar de estad
              }
             // Serial.println(Tof.read_tof()); //For debug

#else 
            Log.msg( F("tof ausente."))
            st_loop = ST_LOOP_OFF_TEST;
#endif 
            
        break;
        
        case ST_LOOP_OFF_TEST:              

            // Setea en el config ensayo terminado. 
            Log.msg( F("ENSAYO TERMINADO, SUERTE!!!"));
            Config.set_st_test( false ); 
            Config.send_test_finish(); //Informa al servidor que termino el ensayo.
            Led.n_blink(3,1000); // 2 blinks cada 1000 ms;          
            st_loop = ST_LOOP_IDLE;             
        break;

        default:
          st_loop = ST_LOOP_INIT;    

           
    }
 #ifdef ST_DEBUG
 Log.msg( F("ST_LOOP= %d"), st_loop );
 #endif //ST_DEBUG
 
}
