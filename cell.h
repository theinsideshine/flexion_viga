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
 
#ifndef CELL_H
#define CELL_H

#include "Arduino.h"
#include <HX711.h>

#define PIN_CELL_FORCE_DAT          6
#define PIN_CELL_FORCE_CLK          5
#define K1_CELL_FORCE              -63932                  //Factor obtenido con Calibrar().
#define STANDARD_WEIGHT_CELL_FORCE  306                    //Peso de calibracion.
#define GET_UNITS                   10                     //Usado en get_units


#define CELL_FORCE_WINDOWS           10                   // Ventana de comparacion para celda de fuerza, en gramos.

class CCell
{
  public:
    CCell();
    void init( void );   
    bool is_force(uint16_t force );
    float read_cell_force( void );
   
    
  private:
  HX711 cell;
  float weight_cell ;
     
};


#endif // CELL_H
