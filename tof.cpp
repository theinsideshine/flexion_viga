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

//TODO: Revisar en el banco real, para ver que conviene devolver 


#include"tof.h"

CTof::CTof()
{


}

bool CTof::init( void )
{
  return ( vl6180.begin());
}

uint8_t CTof::read_tof(void) {

   range = vl6180.readRange();
    
 return (range);
}

bool CTof::read_status (void) {

bool ret_val = false;
  status =  vl6180.readRangeStatus();
  if (status == VL6180X_ERROR_NONE){
    ret_val =true;
  }
  return (ret_val);
}
