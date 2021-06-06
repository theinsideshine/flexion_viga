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


  /*
   * TODO: Al definir metrodos unificar la generacion del pulso  en un metodo.
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
    digitalWrite(PIN_PUL_M1, HIGH);     // Fija condiciones inicial de led indicador alarm, High= Apagado    
    
}

/*
 * Mueve cantidad de milimetro solicitado hacia adelante del motor 1 
 */
 
   
void CMotor::rwd_m1( uint16_t distance )
{
  // Setea el sentido de giro anti-horario, las bobinas deber estar conectadas segun .sch
  
  digitalWrite(PIN_DIR_M1, LOW);
  
   for (uint16_t i = 0; i < ( STEP_PER_MM_M1 * distance ) ; i++) {   //  3200 * 50 

    //Un pulso para un paso.
    digitalWrite(PIN_PUL_M1, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M1, LOW);
    delayMicroseconds(TON_PULSE);
  }
 
}

/*
 * Mueve cantidad de milimetro solicitado hacia adelante del motor 1 
 *  Si los pasos son 6400 por vueltas y el carro puede moverse 500mm  i= 6400 * 500 no puede ser uint_16t
 */


void CMotor::fwd_m1( uint32_t distance )
{
  uint32_t i, coount_cal;
    
   //Setea el sentido de giro  horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M1, HIGH);  

  coount_cal = STEP_PER_MM_M1 * distance ;  // Si distance = 100 mm   tiene que dar 50 vueltas ,paso 2mm , entonces 6400* 50 = 3200.
  
  for ( i = 0; i < coount_cal ; i++) {   
    //Un pulso para un paso i
    digitalWrite(PIN_PUL_M1, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M1, LOW);
    delayMicroseconds(TON_PULSE);
  }
  delay(1000); // Espera 1s ,esta funcione se usar para el movimiento lineal del carro. Este delay es a modo de prueba
  
 
}

/*
 * Mueve para arriba el motor 2 1m y sale 
 * Reemplazada por metordos pwm, para buscar el home
 */
   
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

/*
 * Mueve cantidad de milimetro solicitado hace abajo  del motor 2
 */

// No se probo el cambio de uint32_t distance / coount_cal = STEP_PER_MM_M2 * distance si se probo en fwd_m1

void CMotor::down_m2(  uint32_t distance ){

  uint32_t i, coount_cal;
  
  // Setea el sentido de giro horario, las bobinas deber estar conectadas segun .sch
  digitalWrite(PIN_DIR_M2, HIGH);

  coount_cal = STEP_PER_MM_M2 * distance ;  // Si distance = 100 mm   tiene que dar 50 vueltas ,paso 2mm , entonces 6400* 50 = 3200.
  
   for ( i = 0; i < coount_cal ; i++) {   // Se mueve distance mm para abajo.

    // Un pulso para un paso i.
    digitalWrite(PIN_PUL_M2, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(PIN_PUL_M2, LOW);
    delayMicroseconds(TON_PULSE);
  }
}

// pin 13, 4  980Hz 

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

 
 
