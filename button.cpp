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
 
#include "button.h"

CButton::CButton()
{

}

void CButton::init( void )
{
    pinMode( PIN_LIMIT_M1, INPUT_PULLUP );
    pinMode( PIN_LIMIT_M2, INPUT_PULLUP );
}


/*
 * *****************************FINAL DE CARRERA MOTOR1*******************************
 */

// Resetea el evento de click para evitar falsos disparos.
bool CButton::is_pressed_m1( void )
{
bool ret_val = state_m1;

    state_m1 = false;

    return ret_val;
}

// Retorna true cuando el operador presiono el pulsador de programacion.
// Aplica un mecanismo de antirebote.
void CButton::debounce_m1( void )
{
    state_m1 = (digitalRead( PIN_LIMIT_M1 ) == LOW);

    // Despues que se presiona el pulsador debe permanecer 500 mS liberado.
    if( state_m1 ) {
        if( !Timer_m1.expired( TIME_DEBOUNCE ) ){
          state_m1 = false;
        }

        Timer_m1.start();
    }
}


/*
 * *****************************FINAL DE CARRERA MOTOR2*******************************
 */
// Resetea el evento de click para evitar falsos disparos.
bool CButton::is_pressed_m2( void )
{
bool ret_val = state_m2;

    state_m2 = false;

    return ret_val;
}

// Retorna true cuando el operador presiono el pulsador de programacion.
// Aplica un mecanismo de antirebote.
void CButton::debounce_m2( void )
{
    state_m2 = (digitalRead( PIN_LIMIT_M2 ) == LOW);

    // Despues que se presiona el pulsador debe permanecer 500 mS liberado.
    if( state_m2 ) {
        if( !Timer_m2.expired( TIME_DEBOUNCE ) ){
          state_m2 = false;
        }

        Timer_m2.start();
    }
}
