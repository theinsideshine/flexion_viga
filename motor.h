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


class CMotor
{
  public:
    CMotor(); 
    bool CMotor::init( void );
    void CMotor::rwd_m1( void );
    void CMotor::off_m1( void ); 

  private:
};

#endif // MOTOR_H
