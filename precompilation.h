
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

/*
 *  Para poder estudiar los alcances del tof se pone un modo de operacion .
 *  cuando esto ests en 1 la state machine muestra infromacion del tof
 */


#define ST_MODE_TEST                    0               // Modo de operacion normal, ensayo activado.
#define ST_MODE_TOF                     1               // Modo de operacion TOF, muestra Tof sin promedio.
#define ST_MODE_TOF_AVERAGE             2               // Modo de operacion TOF, muestra Tof con promedio
#define ST_MODE_HOME_M2                 3               // Va al home del motor 2.






/*
 *  existe dos modos de calibracion para las celdas y para el tof 
 *  en el modo celdas saltea el movimiento de los motores y muestra el valor de las celdas y continua el experimento 
 *  en el modo tof ejecutar un funcion de calibracion en lugar del la lectura del tof 
 *  
 */



//#define CALIBRATION_CELL_FORCE   // El ensayo a empezar corre en modo calibracion 

//#define TUNNING_CELL_FORCE // Cuano esta corriendo el modo de calibracion se activa el modo de tuneo.



//#define AVERAGE_TOF // El ensayo a empezar corre en modo calibracion


#ifdef TEST_PROTOTIPE 

#define BUTTON_PRESENT

#define MOTOR_PRESENT
  
#define LED_PRESENT
     
#define CELL_PRESENT   
   
//#define TOF_PRESENT  
  
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
