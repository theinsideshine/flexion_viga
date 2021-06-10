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

/*
 * Arduino Mega

El Timer0 controla las salidas PWM 4 y 13.
El Timer1 controla las salidas PWM 11 y 12.
El Timer2 controla las salidas PWM 9 y 10.
El Timer3 controla las salidas PWM 2, 3 y 5.
El Timer4 controla las salidas PWM 6, 7 y 8.
El Timer5 controla las salidas PWM 44, 45 y 46

pines disponibles de 4 a 13 y de 44 a 46 
Todos los piness 490Hz excepto  4 y 13 980Hz 


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
 
#define PIN_DIR_M1 8            //(Blanco  Proto Blanco  TB6600).  
#define PIN_PUL_M1 13            //(Violeta Proto Naranja TB6600). 

#define STEP_PER_REVOLUTION_M1 6400

#define STEP_PER_REVOLUTION_M2 6400

// las rpm depende de la cantidad de pasos y de la frecuencia del pulso cuando se defina uso dejar la que se usan

// Los Pasos/s ,Rps ,Rpm se calculan en base a la cantidad de pulsos que necesita una vuelta, STEP_PER_REVOLUTION_M1 y  TON_PULSE ,se usan para el movimiento de los milimitros requeridos 

// La busqueda del home incialemnte la hacemos con los pines 4 y 13 que por defecto funcionan a 980 Hz ,

//#define TON_PULSE          2000   // Tiempo en microsegundo on para generacion de pulso de paso F = 250  Hz   250   pasos/s 0.04   Rps  2.34  Rpm. 
//#define TON_PULSE         1500   // Tiempo en microsegundo on para generacion de pulso de paso F = 333.3Hz   333.3 pasos/s 0.05   Rps  3.125 Rpm.
//#define TON_PULSE         1000   // Tiempo en microsegundo on para generacion de pulso de paso F = 500  Hz   500   pasos/s 0.08   Rps  4.68  Rpm. 
//#define TON_PULSE          500   // Tiempo en microsegundo on para generacion de pulso de paso F = 1000 Hz   1000  pasos/s 0.15   Rps  9.35  Rpm.  
#define TON_PULSE          250   // Tiempo en microsegundo on para generacion de pulso de paso F = 2000 Hz   2000  pasos/s 0.3125 Rps  18.75 Rpm.  
//#define TON_PULSE          125   // Tiempo en microsegundo on para generacion de pulso de paso F = 4000 Hz   4000  pasos/s 0.625  Rps  37.5  Rpm
//#define TON_PULSE           63   // Tiempo en microsegundo on para generacion de pulso de paso F = 8000 Hz   8000  pasos/s 1.25   Rps  75    Rpm
//#define TON_PULSE             31   // Tiempo en microsegundo on para generacion de pulso de paso F = 16000Hz   6400  pasos/s 2.5    Rps  150   Rpm

#define PWM_ON             50     // Valor del ciclo de servicio para prender el pwm.
#define PWM_OFF            0      // Valor del ciclo de servicio para apagar  el pwm.




/*
 *       M20 mm ------------------------- STEP_PER_REVOLUTION 
 *       1   mm ------------------------- STEP_PER_REVOLUTION / 20   = 200 /2 =100 STEP POR mm
 *       
 *       1cm   10 mm
 *       50CM  500 mm entonces el parametro distancia debe ser un uint16_t
 *       
 *                      */

#define STEP_PER_MM_M1     STEP_PER_REVOLUTION_M1/M20     // 6400/2 =3200 
#define STEP_PER_MM_M2     STEP_PER_REVOLUTION_M2/M20     // 6400/2 =3200 



/*
 *Motor2.
 */

#ifdef TEST_PROTOTIPE 

#define PIN_DIR_M2 PIN_DIR_M1
#define PIN_PUL_M2 PIN_PUL_M1

#else   

#define PIN_DIR_M2 7
#define PIN_PUL_M2 4 

#endif // TEST_PROTOTIPE


#define CW   0 
#define CCW  1 
#define M1   2
#define M2   3




class CMotor
{
  public:
    CMotor(); 
    bool CMotor::init( void );
    void CMotor::fwd_m1( uint32_t distance ); 
    void CMotor::rwd_m1( uint32_t distance );       
    void CMotor::up_m2 ( uint32_t distance );
    void CMotor::down_m2 ( uint32_t distance ) ;
    
    void CMotor::pwm_on_m1( void ); 
    void CMotor::pwm_off_m1( void );
    void CMotor::pwm_on_m2( void ); 
    void CMotor::pwm_off_m2( void );   
    void CMotor::step_m1_fwd( uint32_t pul );
    void CMotor::step_m1_rwd( uint32_t pul ); 
    void CMotor::step_m2_down( uint32_t pul );
    void CMotor::step_m2_up( uint32_t pul );
  private:
  void CMotor::step_mtr( uint32_t pul ,uint8_t dir ,uint8_t mtr);
};

#endif // MOTOR_H
