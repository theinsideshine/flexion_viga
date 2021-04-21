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
 *      Universidad de la Marina Mercante.
*/  

#include "motor.h"

CMotor::CMotor()
{
   
}

bool CMotor::init( void )
{

    pinMode(PIN_DIR_M1, OUTPUT);
    pinMode(STP_PIN_M1, OUTPUT);
}

// Prende el pwm para retroceder el M1.
void CMotor::rwd_m1( void )
{
  // Set the spinning direction counterclockwise:
  digitalWrite(PIN_DIR_M1, HIGH);
  delayMicroseconds(2000);
  analogWrite(STP_PIN_M1, PWM_ON);
  delay(1000);
}

// Prende el pwm para retroceder el M1.
void CMotor::off_m1( void )
{
  analogWrite(STP_PIN_M1, PWM_OFF); //
  delay(1000);
}
