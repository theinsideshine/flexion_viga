
/**
 * File:   Defincion para soporte de precompilacion 
 *         dependiendo del ambiente de uso, prototipo o final 
 *         se seleccion las macros a definir
 *
 * - Compiler:           Arduino 1.8.13
 * - Supported devices:  Mega
 *
 * \author               MV: luis.villacorta@alumnos.udemm.edu.ar
 *                       LC: leandro.cintioli@alumnos.udemm.edu.ar
 *                       PT: pablo.tavolaro@alumnos.udemm.edu.ar
 *
 * Date:  10-05-2021
 *
 *      Universidad de la Marina Mercante.
 */


 // pines https://www.arduino.cc/en/Hacking/PinMapping2560
 
#ifndef PRECOMPILATION_H
#define PRECOMPILATION_H

#include "Arduino.h"

#define TEST_PROTOTIPE  // Banco de prueba prototipo

//#define CALIBRATION_CELL_FORCE // El ensayo a empezar corre en modo calibracion 

//#define TUNNING_CELL_FORCE // Cuano esta corriendo el modo de calibracion se activa el modo de tuneo.



#ifdef TEST_PROTOTIPE 

#define BUTTON_PRESENT

#define MOTOR_PRESENT
  
#define LED_PRESENT
     
#define CELL_PRESENT   
   
#define TOF_PRESENT  
  
//#define ST_DEBUG
#else                   //Banco de prueba final

#define BUTTON_PRESENT

#define MOTOR_PRESENT
  
#define LED_PRESENT
     
#define CELL_PRESENT   
   
//#define TOF_PRESENT  
  
//#define ST_DEBUG

#endif //TEST_PROTOTIPE


#endif // PRECOMPILATION_H
