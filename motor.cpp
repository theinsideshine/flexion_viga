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
/*
 * Inicializa los pines pul y dir de los dos motores, el pin enable se maneja directamente por hardware pora panico. 
 */


bool CMotor::init( void )
{
    
    pinMode(PIN_DIR_M1, OUTPUT);
    pinMode(PIN_PUL_M1, OUTPUT);   
    pinMode(PIN_DIR_M2, OUTPUT);
    pinMode(PIN_PUL_M2, OUTPUT);
   
    
    digitalWrite(PIN_DIR_M1, HIGH);
    digitalWrite(PIN_PUL_M1, HIGH);     // Fija condiciones inicial de led indicador alarm, High= Apagado  

     digitalWrite(PIN_DIR_M2, HIGH);
    digitalWrite(PIN_PUL_M2, HIGH);     // Fija condiciones inicial de led indicador alarm, High= Apagado

    
}
/*
 *  Mueve la cantidad de passos solicitado en la direccion pedida
 *  De hoja de datos
 *  Tiempo entre  Ena y Dir t1 > 5 us 
 *  Tiempo entre  Dir y Pul t2 > 5 us 
 *  Tiempo min del pulso en alto/bajo t3=t4 >2.5 us  T=5 us F = 200KHz
 *  
 *  uint32_t pul  cantidad de pulsos
 *  uint8_t dir  CW horario CCW anti horario
 *  uint8_t mtr  M1 motor 1 M2 motor2
 */
void CMotor::step_mtr( uint32_t pul ,uint8_t dir ,uint8_t mtr )
{
  uint32_t i;
  uint8_t pin_pul_mtr ,pin_dir_mtr ;

  if ( mtr == M1 ) {
    
      pin_pul_mtr = PIN_PUL_M1;
      pin_dir_mtr = PIN_DIR_M1;
      
  }else if ( mtr == M2 ) {
    
            pin_pul_mtr = PIN_PUL_M2;
            pin_dir_mtr = PIN_DIR_M2;
  }
  
  // Setea el sentido de giro anti-horario, las bobinas deber estar conectadas segun .sch
  
  if ( dir == CCW ){
    
      digitalWrite(pin_dir_mtr, LOW);
    
  }else if ( dir == CW ){
    
            digitalWrite(pin_dir_mtr, HIGH);
  }
  
  delayMicroseconds(T_DIR_PUL);   // Espera t2 > 5 us.
  
   for ( i = 0; i < pul ; i++) {  

    // Un pulso para un paso.    
    digitalWrite(pin_pul_mtr, HIGH);
    delayMicroseconds(TON_PULSE);
    digitalWrite(pin_pul_mtr, LOW);
    delayMicroseconds(TON_PULSE);
  }
 
}

void CMotor::step_m1_fwd( uint32_t pul ){
  step_mtr( pul ,CW ,M1 );  
}

void CMotor::step_m1_rwd( uint32_t pul ){
  step_mtr( pul ,CCW, M1 );  
}

void CMotor::step_m2_down( uint32_t pul ){
  step_mtr( pul ,CW ,M2 );  
}

void CMotor::step_m2_up( uint32_t pul ){
  step_mtr( pul ,CCW, M2 );
  
}


/*
 * Mueve cantidad de milimetro solicitado hacia atras del motor 1 
 */
 
   
void CMotor::rwd_m1( uint32_t distance ){
 uint32_t  count_cal;    

  count_cal = STEP_PER_MM_M1 * distance ;  
  
  step_m1_rwd( count_cal );
   
 
}

/*
 * Mueve cantidad de milimetro solicitado hacia adelante del motor 1 
 *  Si los pasos son 6400 por vueltas y el carro puede moverse 500mm  i= 6400 * 500 no puede ser uint_16t
 */


void CMotor::fwd_m1( uint32_t distance ){
  uint32_t  count_cal;    
  

  count_cal = STEP_PER_MM_M1 * distance ;  
  
  step_m1_fwd( count_cal );
   
}

/*
 * Mueve para arriba el motor 2 1m y sale 
 * Reemplazada por metordos pwm, para buscar el home
 */
   
void CMotor::up_m2( uint32_t distance ){
  uint32_t  count_cal;    

  count_cal = STEP_PER_MM_M1 * distance ;  
  
  step_m2_up( count_cal );
}

/*
 * Mueve cantidad de milimetro solicitado hace abajo  del motor 2
 */

void CMotor::down_m2(  uint32_t distance ){

 uint32_t  count_cal;    

  count_cal = STEP_PER_MM_M1 * distance ;  
  
  step_m2_down( count_cal );
  
}



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

 
 
