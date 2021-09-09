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


#define PIN_TOF_SDA          20             //Este pin  lo usa la clase Adafruit_VL6180X en la IC2.  Cable blanco  
#define PIN_TOF_SCK          21             //Este pin  lo usa la clase Adafruit_VL6180X en la IC2.   Cable azul



#define N_SAMPLES_TOF        20            // Numero de muestras del promedio.  
#define CERO_FLEXION         43            // Cero de referencia para medir la flexion.
//#define TOF_DEBUG                          // Muestra informacion del tof por el puerto serie.
 
class CTof
{
  public:
    CTof();
    bool init( void );
    uint8_t read_tof_cero(void);
    uint8_t get_tof_flexion(void);
    uint32_t get_tof_average(void);
    uint8_t get_tof(void);
    void set_tof_cero(void);
    bool read_status (void);
    bool print_status (void);
    

  private:
  
      uint8_t distance_0 ;
      uint8_t distance_1; 
      uint32_t average ; 
      uint8_t count_sample;
      
          
      uint8_t status;
      uint8_t range;
     
      Adafruit_VL6180X vl6180 = Adafruit_VL6180X(); 
};


#endif // TOF_H
