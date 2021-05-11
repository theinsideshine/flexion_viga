/*
 * File:    File:   Encapsula el control de los motores paso a paso .
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
 * 
 */
#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include "precompilation.h"
//Avance por vuelta 

#define M5    0.8  // 0,8mm per revolution
#define M8    1.25 // 1.25mm per revolution
#define M20   2    // 2 mm per revolution


/*
 *Motor1.
 */
 
#define PIN_DIR_M1 8
#define PIN_PUL_M1 9
#define PIN_EN_M1  10 

#define STEP_PER_REVOLUTION_M1 200
#define STEP_PER_REVOLUTION_M2 200


#define TON_PULSE          1000   //Tiempo en microsegundo on para generacion de pulso de paso F=500Hz


/*
 *       M20 mm ------------------------- STEP_PER_REVOLUTION 
 *       1   mm ------------------------- STEP_PER_REVOLUTION / 20   = 200 /2 =100 STEP POR mm
 *       
 *       1cm   10 mm
 *       50CM  500 mm entonces el parametro distancia debe ser un uint16_t
 *       
 *                      */

#define STEP_PER_MM_M1     STEP_PER_REVOLUTION_M1/M20     //Para no usar micropasos, chequear que sea un entero 
#define STEP_PER_MM_M2     STEP_PER_REVOLUTION_M2/M20     //Para no usar micropasos, chequear que sea un entero 



/*
 *Motor2.
 */

#ifdef TEST_PROTOTIPE 

#define PIN_DIR_M2 PIN_DIR_M1
#define PIN_PUL_M2 PIN_PUL_M1

#else   

#define PIN_DIR_M2 13
#define PIN_PUL_M2 14

#endif // TEST_PROTOTIPE


class CMotor
{
  public:
    CMotor(); 
    bool CMotor::init( void );
    void CMotor::rwd_m1( void );
    void CMotor::off_m1( void ); 
    void CMotor::fwd_m1(uint16_t distance); 
    void CMotor::up_m2();
    void CMotor::down_m2();
    
  private:
};

#endif // MOTOR_H
