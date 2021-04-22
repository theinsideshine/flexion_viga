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
/*
 * En principìo tenemos dos funciones una no- bloqueante para usar esperando el final de carrera desde main void CMotor::rwd_m1( void )
 * y otra bloqueante que se mueve cantidad de mm solicitados 
 * 
 */
// Prende el pwm para retroceder el M1.Fpwm(default aprox 500hz)
   
void CMotor::rwd_m1( void )
{
  // Setea el sentido de giro horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M1, LOW);
  delayMicroseconds(2000);
  analogWrite(STP_PIN_M1, PWM_ON);
  delay(1000);
}

// Prende el pwm para retroceder el M1.
void CMotor::rwd_off_m1( void )
{
  analogWrite(STP_PIN_M1, PWM_OFF); //
  delay(1000);
}

void CMotor::fwd_m1( uint16_t distance )
{
  //Setea el sentido de giro anti- horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M1, HIGH);
  
 
  for (uint16_t i = 0; i < (STEP_PER_MM*distance) ; i++) {   //por cada mm requerido en distancia hago STEP_PER_MM veces  STEP_PER_MM * distance

    //Un pulso para un pasoi
    digitalWrite(STP_PIN_M1, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(STP_PIN_M1, LOW);
    delayMicroseconds(TON_PULSE);
  }
  delay(1000); // espera 1s
 
}
