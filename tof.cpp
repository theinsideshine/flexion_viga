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
  average = 0 ; 
  count_sample = N_SAMPLES_TOF;

}

bool CTof::init( void )
{
  return ( vl6180.begin());
}

/*
 * Lee la referencia de la viga en cero con el Tof.
 */

uint8_t CTof::get_tof(void) {
      
 return (range);
}

/*
 *  
 * Devuelve la diferencia entre la referencia y la medicion
 */
 

uint8_t CTof::get_tof_flexion(void) {  

#ifdef TOF_DEBUG 
    Serial.print("flexion:");
    Serial.println(distance_0-distance_1);
#endif 

    return (distance_0-distance_1);
 
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

uint32_t CTof::get_tof_average(void){


uint32_t ret_val = 0 ;

   while(1) {  // warning: De aca no sale si no hay lectura valida, no es una buena practica. TODO:este metodo debe ser no:bloqueante
    
   
        range  = vl6180.readRange();
        status = vl6180.readRangeStatus();

        if (status == VL6180X_ERROR_NONE) {
      
            if (count_sample==0){      
              count_sample = N_SAMPLES_TOF;
              average = average / N_SAMPLES_TOF;
              ret_val = average;         //  Para usar en calibracion. 
              distance_1 = average;      //  Para usar en calculo de flexion.
              average=0;
              break;       
            }else {
              average = (average + range);
             count_sample = count_sample -1; 
             }
          
        }else {
#ifdef TOF_DEBUG
        print_status ();            // Imprime codifo de error.
#endif
        
        }
  }
    
   return ( ret_val );
  
}

/*
 * Lee el status del tof
 */

bool CTof::read_status (void) {

bool ret_val = false;
  range = vl6180.readRange();
  status =  vl6180.readRangeStatus();
  if (status == VL6180X_ERROR_NONE){
    ret_val =true;
  }
  return (ret_val);;
}


/*
 * imprime el error tof
 */

bool CTof::print_status (void) {

if  ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5)) {
    Serial.println("System error");
  }
  else if (status == VL6180X_ERROR_ECEFAIL) {
    Serial.println("ECE failure");
  }
  else if (status == VL6180X_ERROR_NOCONVERGE) {
    Serial.println("No convergence");
  }
  else if (status == VL6180X_ERROR_RANGEIGNORE) {
    Serial.println("Ignoring range");
  }
  else if (status == VL6180X_ERROR_SNR) {
    Serial.println("Signal/Noise error");
  }
  else if (status == VL6180X_ERROR_RAWUFLOW) {
    Serial.println("Raw reading underflow");
  }
  else if (status == VL6180X_ERROR_RAWOFLOW) {
    Serial.println("Raw reading overflow");
  }
  else if (status == VL6180X_ERROR_RANGEUFLOW) {
    Serial.println("Range reading underflow");
  }
  else if (status == VL6180X_ERROR_RANGEOFLOW) {
    Serial.println("Range reading overflow");
  }
  delay(50);

}
