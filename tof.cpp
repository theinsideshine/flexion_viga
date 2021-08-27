/**
   File:   Encapsula el control del sensor de distancia con tecnologia TOF.

           
          

   - Compiler:           Arduino 1.8.13
   - Supported devices:  Mega

   \author               MV: luis.villacorta@alumnos.udemm.edu.ar
                         LC: leandro.cintioli@alumnos.udemm.edu.ar
                         PT: pablo.tavolaro@alumnos.udemm.edu.ar

   Date:  25-04-2021

        Universidad de la Marina Mercante.
*/


#include"tof.h"

CTof::CTof()
{
  distance_0 = 80;
  distance_1 = 0 ;

}

bool CTof::init( void )
{
  return ( vl6180.begin());
}

/*
 * Lee la referencia de la viga en cero con el Tof.
 */

uint8_t CTof::read_tof_cero(void) {

   distance_0 = vl6180.readRange();
   Serial.println(distance_0); //For debug
    
 return (distance_0);
}

/*
 * Lee la felxion de la viga  con el Tof.
 * Devuelve la diferencia entre la referencia y la medicion
 */
 

uint8_t CTof::read_tof_flexion(void) {

   distance_1 = vl6180.readRange();
   Serial.println(distance_1); //For debug 
   
 return (distance_0-distance_1);
}


/*
 * Lee el Tof.
 */
 
uint8_t CTof::read_tof(void) {

   distance_1 = vl6180.readRange();
   
   return (distance_1);
}

/*
 * Carga la referencia por macros.
 */


 void  CTof::set_tof_cero(void){
  
  distance_0 = CERO_FLEXION ;
  
 }

/*
 * Lee el Tof con promedio.
 */

uint8_t CTof::read_tof_average(void){

uint8_t ret_val = 0; 
uint16_t average_tof = 0;     


    status =  VL6180X_ERROR_NONE;  //sacar 
    
    for ( uint8_t i = 0; i< N_SAMPLES_TOF;  i++ ){
         
         //ret_val =vl6180.readRange();  
         ret_val = 50+i;          //sacar
         average_tof = average_tof + ret_val ; 
         while( !(status == VL6180X_ERROR_NONE) ){
            //status =  vl6180.readRangeStatus();
           }
         
        }
      
    
    ret_val = average_tof / N_SAMPLES_TOF ;

    return ( ret_val );
  
}

/*
 * Lee el status del tof
 */

bool CTof::read_status (void) {

bool ret_val = false;
  status =  vl6180.readRangeStatus();
  if (status == VL6180X_ERROR_NONE){
    ret_val =true;
  }
  return (ret_val);;
}
