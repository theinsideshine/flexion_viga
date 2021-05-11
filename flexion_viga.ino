/**
 * File:   
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


//TODO: _Investigar el tema de usar los finales de carrera como interrupciones de panico 
//      usar el formato 
//      {
//        cmd:'start',
//       [{distance:'500'},{force:'11'}  ]
//       } 


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
#define ST_LOOP_INIT_M1                 2     // Busca la referencia del Motor 1.
#define ST_LOOP_INIT_M2                 3     // Busca la referencia del Motor 1.
#define ST_LOOP_POINT_M1                4     // Se mueve m1 cantidada de pasos requeridos en mm.
#define ST_LOOP_FORCE_M2                5     // Se mueve m2 hasta que encuentra la fuerza requrida en kilos.
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





// Inicializa los perfericos del cartel.
void setup()
{
   Log.init( Config.get_log_level() );  
   Serial.println("Init Serial"); 
 /*
  *  Para activar la visualisacion  enviar por serie {log_level:'1'}    
  */
  
   Log.msg( F("Ensayo viga simplemente apoyada - %s"), FIRMWARE_VERSION ); 
   Log.msg( F("UDEMM - 2021") );   
   
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
 
 }


// Loop de control del Ensayo viga simplemente apoyada
void loop()
{
static CTimer   Timer;
static uint8_t  st_loop = ST_LOOP_INIT;  
static float peso = 0 ;

    //Lee el finales de carrera de cada motor
    Button.debounce_m1();
    Button.debounce_m2();
    
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
           st_loop = ST_LOOP_INIT_M1;
           Log.msg( F("Inicializando motor1. Esperando final de carrera M1"));
        }
        break; 

            //Mueve en direccion rewind el motor1 hasta que se presiones el final de carrera m1.

        case ST_LOOP_INIT_M1:
             
            
            if (Button.is_pressed_m1() ) {             

              st_loop = ST_LOOP_INIT_M2; 
              Log.msg( F("Inicializando motor2. Esperando final de carrera M2"));
              delay(1000); // Espera para pasar de estado.                                     
            }else {            
             
             Motor.rwd_m1(); 
            }
              
        break;  

            //Mueve en direccion up el motor2 hasta que se presiones el final de carrera m2.
            
         case ST_LOOP_INIT_M2:            
            
            
            if (Button.is_pressed_m2() ) {             
              
              st_loop = ST_LOOP_POINT_M1;   
              Log.msg( F("Moviendo el motor 1 cantidad de milimitros "));   
              delay(1000); // Espera para pasar de estado.               
            }else {
            
             Motor.up_m2(); 
            }
              
        break;

        //Mueve el motor1 en direccion forward ,la distance en mm de la configuracion.
       case ST_LOOP_POINT_M1:
              
              Motor.fwd_m1(Config.get_distance()); 
              Log.msg( F("Moviendo el motor 2 haste leer la fuerza configurada "));             
              st_loop = ST_LOOP_FORCE_M2;
              Led.on(); //prende led apoyar peso
              delay(1000); // Espera para pasar de estado 
        break;

          //Mueve el moto2r en direccion down ,hasta que se aplique la fuerza en gramos de la configuracion.
         case ST_LOOP_FORCE_M2:
                            
              Cell.read_cell_force();  //TODO: presupone que la celda esta sin carga al arrancar no debe tener peso la celda
              if ( Cell.is_force(Config.get_force())) {
                                
                 Log.msg( F("Force:Ok "));
                 Led.off(); //apaga led 
                 st_loop = ST_LOOP_GET_R1 ;
                 delay(1000); // Espera para pasar de estado                  
              }else {
                Motor.down_m2();
              }
                              
        break;

          //Lee la fuerza de reaccion 1 y la guarda en la configuracion.
        case ST_LOOP_GET_R1:

                Log.msg( F("Lectura de fuerza de reaccion 1 en 3 segundos ponga el peso"));
                 delay(3000); //Tiempo para poner otro peso (DEBUG).
                
                Config.set_reaction1(Cell.read_cell_reaction1());
                      
               st_loop = ST_LOOP_GET_R2;
               delay(1000); // Espera para pasar de estado.

        break;

        //Lee la fuerza de reaccion 2 y la guarda en la configuracion.
        case ST_LOOP_GET_R2:

                Log.msg( F("Lectura de fuerza de reaccion 1 en 3 segundos ponga el peso"));
                delay(3000); //tiempo para poner otgro peso(DEBUG).
                
                Config.set_reaction2(Cell.read_cell_reaction2());
                      
               st_loop = ST_LOOP_GET_FLEXION;
               delay(1000); // Espera para pasar de estado 

        break;

        //Lee la distancia de flexion y la guarda en la configuracion.
        case ST_LOOP_GET_FLEXION:
        
              Log.msg( F("Lectura del tof"));              
              if (Tof.read_status()){
                 Config.set_flexion(Tof.read_tof());
                 st_loop = ST_LOOP_OFF_TEST;
                 delay(1000); // Espera para pasar de estad
              }
            
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
