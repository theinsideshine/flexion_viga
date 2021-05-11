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
    pinMode(PIN_EN_M1, OUTPUT);
    pinMode(PIN_DIR_M1, OUTPUT);
    pinMode(PIN_PUL_M1, OUTPUT);
    digitalWrite(PIN_EN_M1, HIGH); //
}


//mueve para atras 1m y sale

   
void CMotor::rwd_m1( void )
{
  // Setea el sentido de giro anti-horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M1, LOW);
  
   for (uint16_t i = 0; i < (STEP_PER_MM_M1) ; i++) {   //Se mueve 1mm

    //Un pulso para un paso.
    digitalWrite(PIN_PUL_M1, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M1, LOW);
    delayMicroseconds(TON_PULSE);
  }
 
}


/*
 * Mueve cantidad de milimetro solicitado 
 */

void CMotor::fwd_m1( uint16_t distance )
{
  //Setea el sentido de giro  horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M1, HIGH);
  
 
  for (uint16_t i = 0; i < (STEP_PER_MM_M1*distance) ; i++) {   //por cada mm requerido en distancia hago STEP_PER_MM veces  STEP_PER_MM_M1 * distance

    //Un pulso para un pasoi
    digitalWrite(PIN_PUL_M1, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M1, LOW);
    delayMicroseconds(TON_PULSE);
  }
  delay(1000); // espera 1s
 
}

//mueve para arriba el motor2 1m y sale

   
void CMotor::up_m2( void )
{
  // Setea el sentido de giro anti-horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M2, LOW);
  
   for (uint16_t i = 0; i < (STEP_PER_MM_M1) ; i++) {   //se mueve 1mm para arriba

    //Un pulso para un pasoi
    digitalWrite(PIN_PUL_M2, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M2, LOW);
    delayMicroseconds(TON_PULSE);
  }
}

//mueve para arriba el motor2 1m y sale

void CMotor::down_m2( void )
{
  // Setea el sentido de giro horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M2, HIGH);
  
   for (uint16_t i = 0; i < (STEP_PER_MM_M2*2) ; i++) {   //se mueve 2mm para abajo

    //Un pulso para un pasoi
    digitalWrite(PIN_PUL_M2, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M2, LOW);
    delayMicroseconds(TON_PULSE);
  }  
 
}
