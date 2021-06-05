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
  

// El motor nema 23HS5628 es de 2.8 A rms 
 
// Tb6600 Investigacion sujeta a corrobar por metodo empirico 

// Microstep config                   S1  S2  S3
//                                    OFF OFF OFF   32 Micro step 6400 pulso/rev (uso de Egresado mecanica)
// Microstep config                   S1  S2  S3 
//                                    ON  ON  OFF   1  Micro step 200  pulso/rev

// Currente Pk                        S4  S5  S6   
//                                    ON  ON  OFF  1.7A peak (uso de Egresado mecanica)
//                                    OFF OFF  OFF  4.0A peak 
 
#include "motor.h"

CMotor::CMotor()
{
   
}

bool CMotor::init( void )
{
    
    pinMode(PIN_DIR_M1, OUTPUT);
    pinMode(PIN_PUL_M1, OUTPUT);
    digitalWrite(PIN_DIR_M1, HIGH);
    digitalWrite(PIN_PUL_M1, HIGH);
    
    
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
  
   for (uint16_t i = 0; i < (STEP_PER_MM_M2) ; i++) {   //se mueve 1mm para arriba

    //Un pulso para un paso
    digitalWrite(PIN_PUL_M2, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M2, LOW);
    delayMicroseconds(TON_PULSE);
  }
}

//mueve para arriba el motor2 2m y sale

void CMotor::down_m2( void ){
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

// pin 13 980Hz 

 void CMotor::pwm_on_m1( void ){
    // Setea el sentido de giro anti-horario, las bobinas deber estar conectadas segun .sch
   digitalWrite(PIN_DIR_M1, LOW);
   
   //Prende el pwm.
   analogWrite(PIN_PUL_M1,PWM_ON);
  
 }

  void CMotor::pwm_off_m1( void ){

    //Apaga el pwm.
    analogWrite(PIN_PUL_M1,PWM_OFF);
  
 }

 void CMotor::pwm_on_m2( void ){
    // Setea el sentido de giro anti-horario, las bobinas deber estar conectadas segun .sch
   digitalWrite(PIN_DIR_M2, LOW);
   
   //Prende el pwm.
   analogWrite(PIN_PUL_M2,PWM_ON);
  
 }

  void CMotor::pwm_off_m2( void ){

    //Apaga el pwm.
    analogWrite(PIN_PUL_M2,PWM_OFF);
  
 }

 
 
