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
#include "precompilation.h"
#include <HX711.h>

#ifdef TEST_PROTOTIPE 

#define PIN_CELL_FORCE_DAT          6
#define PIN_CELL_FORCE_CLK          5
#define K1_CELL_FORCE              -63932                  //Factor obtenido con Calibrar().
#define STANDARD_WEIGHT_CELL_FORCE  306                    //Peso de calibracion.

#define PIN_CELL_REACTION1_DAT          PIN_CELL_FORCE_DAT
#define PIN_CELL_REACTION1_CLK          PIN_CELL_FORCE_CLK
#define K1_CELL_REACTION1               K1_CELL_FORCE                  
#define STANDARD_WEIGHT_CELL_REACTION1  STANDARD_WEIGHT_CELL_FORCE   


#define PIN_CELL_REACTION2_DAT          PIN_CELL_FORCE_DAT
#define PIN_CELL_REACTION2_CLK          PIN_CELL_FORCE_CLK
#define K1_CELL_REACTION2               K1_CELL_FORCE                  
#define STANDARD_WEIGHT_CELL_REACTION2  STANDARD_WEIGHT_CELL_FORCE   

#else 

#define PIN_CELL_FORCE_DAT          10
#define PIN_CELL_FORCE_CLK          9
#define K1_CELL_FORCE              -63932                  //Factor obtenido con Calibrar().
#define STANDARD_WEIGHT_CELL_FORCE  306                    //Peso de calibracion.

#define PIN_CELL_REACTION1_DAT          6
#define PIN_CELL_REACTION1_CLK          5
#define K1_CELL_REACTION1               333
#define STANDARD_WEIGHT_CELL_REACTION1  31   


#define PIN_CELL_REACTION2_DAT          4
#define PIN_CELL_REACTION2_CLK          3
#define K1_CELL_REACTION2               334                  
#define STANDARD_WEIGHT_CELL_REACTION2  21  


#endif //TEST_PROTOTIPE


#define GET_UNITS                    10                   // Cantidad de lecturas a realizar.
#define CELL_FORCE_WINDOWS           10                   // Ventana de comparacion para celda de fuerza, en gramos.

class CCell
{
  public:
    CCell();
    void init( void );   
    bool is_force(uint16_t force );
    void read_cell_force( void );
    float read_cell_reaction1(void);
    float read_cell_reaction2(void);
   
    
  private:
  HX711 cell_force;
  float weight_cell_force ;
  HX711 cell_reaction1;
  float weight_cell_reaction1;
  HX711 cell_reaction2;
  float weight_cell_reaction2;
  
     
};


#endif // CELL_H
