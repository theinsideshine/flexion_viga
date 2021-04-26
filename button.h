/**
 * File:   Clase que controla los finales de carrera
 *
 * - Compiler:           Arduino 1.8.13
 * - Supported devices:  Mega
 *
 * \author               MV: luis.villacorta@alumnos.udemm.edu.ar
 *                       LC: leandro.cintioli@alumnos.udemm.edu.ar
 *                       PT: pablo.tavolaro@alumnos.udemm.edu.ar
 *
 * Date:  21-04-2021
 *
 *      Universidad de la Marina Mercante.
 */
#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"
#include "timer.h"

#define PIN_LIMIT_M1                  2       // Pin del pulsador de configuracion.
#define PIN_LIMIT_M2                  14      // Pin del pulsador de configuracion.

#define TIME_DEBOUNCE                 500       // Tiempo de espera en ms.

class CButton
{
  public:
    CButton();
    void init( void );
    bool is_pressed_m1( void );
    void debounce_m1( void );
    bool is_pressed_m2( void );
    void debounce_m2( void );
    
  private:
      CTimer Timer_m1;
      bool state_m1;                   // TRUE presionado.
      CTimer Timer_m2;
      bool state_m2;                   // TRUE presionado.
};

#endif // BUTTON_H
