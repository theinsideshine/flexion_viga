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
  float scale; 
 
   cell_force.begin(PIN_CELL_FORCE_DAT, PIN_CELL_FORCE_CLK);              // Inicializa la clase para controlar la balanza de fuerza aplicada
   cell_force.tare(20);  //20 mediciones

#ifdef CALIBRATION_CELL_FORCE

  #ifdef TUNNING_CELL_FORCE

        scale=K1_CELL_FORCE/STANDARD_WEIGHT_CELL_FORCE;
        cell_force.set_scale(scale);
  #else
  
      // Nada.      
      
  #endif
       
#else 
       
   scale=K1_CELL_FORCE/STANDARD_WEIGHT_CELL_FORCE;
   cell_force.set_scale(scale);
   
#endif  // CALIBRATION_CELL_FORCE
   

   cell_reaction1.begin(PIN_CELL_REACTION1_DAT, PIN_CELL_REACTION1_CLK);  // Inicializa la clase para controlar la balanza de reaccion 1
   cell_reaction1.tare(20);  
   scale=K1_CELL_REACTION1/STANDARD_WEIGHT_CELL_REACTION1;
   cell_reaction1.set_scale(scale);
   
   cell_reaction2.begin(PIN_CELL_REACTION2_DAT, PIN_CELL_REACTION2_CLK);  // Inicializa la clase para controlar la balanza de reaccion 2
   cell_reaction2.tare(20);  
   scale=K1_CELL_REACTION2/STANDARD_WEIGHT_CELL_REACTION2;
   cell_reaction2.set_scale(scale);

   
   
}


// Lee la fuerza de accion

void CCell::read_cell_force( void ){
  
 weight_cell_force = cell_force.get_units(GET_UNITS);
 Serial.print(" Fuerza aplicada: ");  
 Serial.println( weight_cell_force,1 );

 
#ifdef CALIBRATION_CELL_FORCE 

 Serial.print(" Constante de celda de fuerza con peso patron= Fr1+ Fr2 : "); 
 Serial.println( weight_cell_force,1 ); //For calibration add macro precompilation

#endif // CALIBRATION_CELL_FORCE
 
}

// Devuelve el valor de la fuerza de accion.

float CCell::get_read_force( void ){

  return (weight_cell_force);
}

//Si la fuerza esta dentro de la ventana de comparacion devuel true.

bool CCell::is_force(uint16_t force  ){  //esta en gramos

bool state = false ;

    /*
    if ( ( weight_cell_force > (force -  CELL_FORCE_WINDOWS  ) ) &&
         ( weight_cell_force < (force +  CELL_FORCE_WINDOWS  ) )
       ){
      state = true;
    }*/



     if (  weight_cell_force > (force -  CELL_FORCE_WINDOWS  )  ){
      state = true;
    }
    
    
    return ( state );
      
}

//Lee la fuerza de reaccion1.

float CCell::read_cell_reaction1( void ){
  
  weight_cell_reaction1 = cell_reaction1.get_units(GET_UNITS);

#ifdef CALIBRATION_CELL_FORCE 

 Serial.print(" Fuerza reaccion 1: ");  
 Serial.println( weight_cell_reaction1,1 ); 
 
#endif // CALIBRATION_CELL_FORCE
  
  return(weight_cell_reaction1);

}


//Lee la fuerza de reaccion2.

float CCell::read_cell_reaction2( void ){
  
  weight_cell_reaction2 = cell_reaction2.get_units(GET_UNITS);
  
#ifdef CALIBRATION_CELL_FORCE 

 Serial.print(" Fuerza reaccion 2: "); 
 Serial.println( weight_cell_reaction2,1 ); 
 
#endif   // CALIBRATION_CELL_FORCE
 
  return(weight_cell_reaction2);
 
}
