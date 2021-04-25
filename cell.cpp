/**
 * File:   Encapsula el control del la celdas de carga.
 *
 * - Compiler:           Arduino 1.8.13
 * - Supported devices:  Mega
 *
 * \author               MV: luis.villacorta@alumnos.udemm.edu.ar
 *                       LC: leandro.cintioli@alumnos.udemm.edu.ar
 *                       PT: pablo.tavolaro@alumnos.udemm.edu.ar
 *
 * Date:  04-25-2021
 *
 *      Universidad de la Marina Mercante.
 */

#include"cell.h"

CCell::CCell()
{
   
}

 void CCell::init( void )
{
    cell.begin(PIN_CELL_FORCE_DAT, PIN_CELL_FORCE_CLK);  // Inicializa la clase para controlar la balanza
    float scale;      
    cell.set_scale(); //La escala por defecto es 1 este no va creo!
   cell.tare(20);  //20 mediciones
   scale=K1_CELL_FORCE/STANDARD_WEIGHT_CELL_FORCE;
   cell.set_scale(scale);
}

float CCell::read_cell_force( void ){
  
 weight_cell = cell.get_units(GET_UNITS);
  Serial.println( weight_cell,1 );
 
 
 
 
}

bool CCell::is_force(uint16_t force  ){  //esta en gramos

bool state = false ;

    if ( ( weight_cell > (force -  CELL_FORCE_WINDOWS  ) ) &&
         ( weight_cell < (force +  CELL_FORCE_WINDOWS  ) )
       ){
      state = true;
    }
    
    return ( state );
      
}
