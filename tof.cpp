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


}

bool CTof::init( void )
{
  return ( vl.begin());
}

uint8_t CTof::read_tof(void) {

  if (status == VL6180X_ERROR_NONE){
     range = vl.readRange();
    Serial.println( range, 1 );  
  }else {
     Serial.println( "ERROR_TOF" ); 
     range = status;
  }
 return (range);
}

uint8_t CTof::read_status (void) {
  
  status =  vl.readRangeStatus();
  return (status);
}
