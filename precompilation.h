
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
 *  existe dos modos de calibracion para las celdas y para el tof 
 *  en el modo celdas saltea el movimiento de los motores y muestra el valor de las celdas y continua el experimento 
 *  en el modo tof ejecutar un funcion de calibracion en lugar del la lectura del tof 
 *  
 */



#define CALIBRATION_CELL_FORCE   // El ensayo a empezar corre en modo calibracion 

//#define ST_DEBUG       // Muestra informacion de la maquina de estado por el puerto serie.


#ifdef TEST_PROTOTIPE 

#define BUTTON_PRESENT

#define MOTOR_PRESENT
  
#define LED_PRESENT
     
#define CELL_PRESENT   
   
//#define TOF_PRESENT  
  

#else                   //Banco de prueba final

#define BUTTON_PRESENT

#define MOTOR_PRESENT
  
#define LED_PRESENT
     
#define CELL_PRESENT   
   
#define TOF_PRESENT  
   

#endif //TEST_PROTOTIPE


#endif // PRECOMPILATION_H
