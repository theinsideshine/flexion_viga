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


#define PIN_DIR_M1 8
#define STP_PIN_M1 9
#define STEP_PER_REVOLUTION 200
#define PWM_ON             128
#define PWM_OFF            0
#define TIME_MS            10000

#define TON_PULSE          1000   //Tiempo en microsegundo on para generacion de pulso de paso F=500Hz


#define M5    0.8  // 0,8mm per revolution
#define M8    1.25 // 1.25mm per revolution
#define M20   2    // 2 mm per revolution

/*
 *       M20 mm ------------------------- STEP_PER_REVOLUTION 
 *       1   mm ------------------------- STEP_PER_REVOLUTION / 20   = 200 /2 =100 STEP POR mm
 *       
 *       1cm   10 mm
 *       50CM  500 mm entonces el parametro distancia debe ser un uint16_t
 *       
 *                      */

#define STEP_PER_MM     STEP_PER_REVOLUTION/M20     //Para no usar micropasos, chequear que sea un entero 

 

class CMotor
{
  public:
    CMotor(); 
    bool CMotor::init( void );
    void CMotor::rwd_m1( void );
    void CMotor::rwd_off_m1( void ); 
    void CMotor::fwd_m1(uint16_t distance); 
    

  private:
};

#endif // MOTOR_H
