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
}

// Resetea el evento de click para evitar falsos disparos.
bool CButton::is_pressed( void )
{
bool ret_val = state;

    state = false;

    return ret_val;
}

// Retorna true cuando el operador presiono el pulsador de programacion.
// Aplica un mecanismo de antirebote.
void CButton::debounce( void )
{
    state = (digitalRead( PIN_LIMIT_M1 ) == LOW);

    // Despues que se presiona el pulsador debe permanecer 500 mS liberado.
    if( state ) {
        if( !Timer.expired( TIME_DEBOUNCE ) ){
          state = false;
        }

        Timer.start();
    }
}
