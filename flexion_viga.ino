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


#include "log.h"
#include "cfg.h"
#include "timer.h"
#include "button.h"
#include "motor.h"
#include "tof.h"



#define ST_LOOP_INIT                    0     // Inicializa el programa (carga la configuracion).
#define ST_LOOP_IDLE                    1     // Espera la recepcion por comando.
#define ST_LOOP_INIT_M1                 2     // Busca la referencia del Motor 1.
#define ST_LOOP_INIT_M2                 3     // Busca la referencia del Motor 1.
#define ST_LOOP_POINT_M1                4     // Se mueve cantidada de pasos requeridos en mm.
#define ST_LOOP_OFF_TEST                5     // Termino el ensayo.



Clog    Log;
CConfig Config;
CButton Button;
CMotor   Motor;


/*
 * Se debe pasar a  clase button
 */

#define PIN_LIMIT_M1                   2      //Pin de la interrupcion final de carrera motor1.
#define PIN_LIMIT_M2                   21      //Pin de la interrupcion final de carrera motor2.Cuando lo use en 3 ,metia ruido por la pata 2
#define TIME_DEBOUNCE                  500    //Tiempo de debounce en ms.

volatile bool state_val_m1 = false;           //Estado de la variable de estado del final de carrera m1.

volatile bool state_val_m2 = false;           //Estado de la variable de estado del final de carrera m2.
 
static long start_time_m1 = 0;                //Variable para el debounce dentro de la isr inal de carrera m1. 
static long start_time_m2 = 0;                //Variable para el debounce dentro de la isr inal de carrera m2.


// Inicializa los perfericos del cartel.
void setup()
{
        
   Log.init( Config.get_log_level() );
   Button.init();
  

   /*
    *  Para activar la visualisacion  enviar por serie {log_level:'1'}    
    */

    Log.msg( F("Ensayo viga simplemente apoyada - %s"), FIRMWARE_VERSION ); 
    
    Log.msg( F("UDEMM - 2021") );
   
    Log.msg( F("Sistema inicializado correctamente") );
}


// Loop de control del Ensayo viga simplemente apoyada
void loop()
{
static CTimer   Timer;
static uint8_t  st_loop = ST_LOOP_INIT;  

    //Lee el final de carrera  
    Button.debounce_m1();
    Button.debounce_m2();

    
    /*
     * TODO:cuando el experimento esta  en curso en release debera eliminar esta linea.
     * 
     */
    // Verifica si el host envio un JSON con parametros a procesar.
    Config.host_cmd();

    // Actualiza el nivel de log para detener en tiempo real el envio de parametros.
    Log.set_level( Config.get_log_level() );  

          
    
    
    switch( st_loop ) {
        // Carga los valores de la configuracion y pasa al estado temporizado
        // donde espera que el usuario configure la distancia de peligro.
        case ST_LOOP_INIT:
            st_loop = ST_LOOP_IDLE; 
        break;   

        case ST_LOOP_IDLE:

        if (Config.get_st_test()== true ){
          
          
          st_loop = ST_LOOP_INIT_M1;
        }
        break; 

        case ST_LOOP_INIT_M1:

             
            //Espera que el final de carrera se presione para parar el M1  
            if (Button.is_pressed_m1() ) {             

              st_loop = ST_LOOP_INIT_M2; 
              delay(1000); // Espera para pasar de estado                       
            }else {
            
             Motor.rwd_m1(); 
            }
              
        break;  


         case ST_LOOP_INIT_M2:

             
            //Espera que el final de carrera se presione para parar el M1  
            if (Button.is_pressed_m2() ) {             
                delay(1000); // Espera para pasar de estado 
              st_loop = ST_LOOP_POINT_M1;                        
            }else {
            
             Motor.up_m2(); 
            }
              
        break;

       case ST_LOOP_POINT_M1:

              //mueve el motor distance en mm
              Motor.fwd_m1(Config.get_distance());
              delay(1000); // Espera para pasar de estado 
              st_loop = ST_LOOP_OFF_TEST;
        break;

        case ST_LOOP_OFF_TEST:              

            // setea en el config ensayo terminado. 
            Config.set_st_test( false );  
                      
            st_loop = ST_LOOP_IDLE; 
            
        break;

        default:
          st_loop = ST_LOOP_INIT;    

           
    }
    Log.msg( F("ST_LOOP= %d"), st_loop );
}
