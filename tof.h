/**
 * File:   Encapsula el control del sensor de distancia con tecnologia TOF.
 *
 * - Compiler:           Arduino 1.8.13
 * - Supported devices:  Mega
 *
 * \author               MV: luis.villacorta@alumnos.udemm.edu.ar
 *                       LC: leandro.cintioli@alumnos.udemm.edu.ar
 *                       PT: pablo.tavolaro@alumnos.udemm.edu.ar
 *
 * Date:  25-04-2021
 *
 *      Universidad de la Marina Mercante.
 */
 
#ifndef TOF_H
#define TOF_H

#include "Arduino.h"
#include "Adafruit_VL6180X.h"


#define PIN_TOF_SDA          20             //Este pin  lo usa la clase Adafruit_VL6180X en la IC2.
#define PIN_TOF_SCK          21             //Este pin  lo usa la clase Adafruit_VL6180X en la IC2.


#define N_SAMPLES_TOF        10            // Numero de muestras del promedio. 
 
#define CERO_FLEXION         54            // Cero de referencia para medir la flexion
 
class CTof
{
  public:
    CTof();
    bool init( void );
    uint8_t read_tof_cero(void);
    uint8_t read_tof_flexion(void);
    uint8_t read_tof_average(void);
    uint8_t read_tof(void);
    void set_tof_cero(void);
    bool read_status (void);
    

  private:
  
      uint8_t distance_0 ;
      uint8_t distance_1; 
      
          
      uint8_t status;
      Adafruit_VL6180X vl6180 = Adafruit_VL6180X(); 
};


#endif // TOF_H
