/*
 * File:   Clase para controlar la confirguracion en la EEPROM.
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
 
#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"
#include "precompilation.h"
#include <ArduinoJson.h>

#define FIRMWARE_VERSION                "2.0.04"  // Se agrego parametro st_mode para ejecutar acciones 
                                                  // {st_mode:'0'}         st_mode       ST_MODE_TEST                    0  ensayo activado.
                                                  //                                     ST_MODE_TOF                     1  Modo de operacion TOF, muestra Tof sin promedio.
                                                  //                                     ST_MODE_TOF_AVERAGE             2  Modo de operacion TOF, muestra Tof con promedio.
                                                  //                                     ST_MODE_HOME_M2                 3  Va al home del motor 2. 
                                                  // Se agrego calculo del promedio para el tof;
                                                  // Se agrego lectura del tof unicamente ( sin calculo de flexion)
                                                  // Se agrego set_cero_flexion ,usa una macro para el cero de la flexion
                                                  // Se agrego soporte para contar los pasos del motor 2 desde el home2 hasta la Fforce
                                                  // se agrego correcion del error por contaccion en el motor 2 luego del home
                                                  

                                                  //TODO: LIMITES DE FUERZA APLICADA
                                                  
//#define EEPROM_ADDRESS_CONFIG         4       // Direccion en la epprom donde se almacena la configuracion.
#define MAGIC_NUMBER                    20    // Numero magico para detectar memoria sin inicializar.


#define DISTANCE_DEFAULT                499     // Distancia por defecto donde se aplica la fuerza 100 mm.
#define FORCE_DEFAULT                   10000   // Fuerza por defecto a aplicar Gramos.
#define REACTION1_DEFAULT               0       //  Fuerza de reaccion 1 por defecto.
#define REACTION2_DEFAULT               0       //  Fuerza de reaccion 2 por defecto.
#define FLEXION_DEFAULT                 0       //  Flexion por defecto. 

#define ST_TEST_DEFAULT                 0              //  Estado del test pòr defecto.
#define ST_MODE_DEFAULT                 ST_MODE_TEST   //  Modo de operacion del sistema.                                                                                       



// Mapa de direcciones de los campos de configuracion en la EEPROM.
#define EEPROM_ADDRESS_MAGIC_NUMBER     0
#define EEPROM_ADDRESS_DISTANCE        (EEPROM_ADDRESS_MAGIC_NUMBER + sizeof(uint8_t))
#define EEPROM_ADDRESS_FORCE           (EEPROM_ADDRESS_DISTANCE + sizeof(uint16_t))
#define EEPROM_ADDRESS_REACTION_1      (EEPROM_ADDRESS_FORCE + sizeof(float))
#define EEPROM_ADDRESS_REACTION_2      (EEPROM_ADDRESS_REACTION_1 + sizeof(float))
#define EEPROM_ADDRESS_FLEXION         (EEPROM_ADDRESS_REACTION_2 + sizeof(float))
#define EEPROM_ADDRESS_LOG_LEVEL       (EEPROM_ADDRESS_FLEXION + sizeof(uint8_t))
#define EEPROM_ADDRESS_ST_TEST         (EEPROM_ADDRESS_LOG_LEVEL + sizeof(uint8_t))  
#define EEPROM_ADDRESS_ST_MODE        (EEPROM_ADDRESS_ST_TEST + sizeof(uint8_t))  //este valor es  uint_8 


class CConfig
{
  public:
    CConfig();
    uint16_t get_distance( void );
    void set_distance( uint16_t );
    
    float get_force( void );
    void set_force( float ); 

    float get_reaction1( void );
    void set_reaction1( float ); 

    float get_reaction2( void );
    void set_reaction2( float ); 

    uint8_t get_flexion( void );
    void set_flexion( uint8_t ); 

    uint8_t get_log_level( void );
    void set_log_level( uint8_t enable );

    uint8_t get_st_test( void );
    void set_st_test( uint8_t enable );

    uint8_t get_st_mode( void );
    void set_st_mode( uint8_t mode ); 

    void host_cmd( void );
    void send_test_finish(void);

  private:
    uint8_t log_level;          // 0 = log de informacion de control desactivada.
    uint8_t st_test;            // Estado del ensayo 
    uint8_t st_mode;            // Modo del ensayo


    uint16_t distance;            // Distancia donde se aplica la fuerza.
    float force;              // Fuerza a aplicar.
    float reaction1;          // Fuerza de reaccion 1.
    float reaction2;          // Fuerza de reaccion 2.
    uint8_t flexion;           // Distancia de flexion.
    
    void send_all_params( JsonDocument& );
    void send_version( JsonDocument& );
    void send_ok( JsonDocument& );
    void send_ack( JsonDocument& );
    void send_status( JsonDocument& doc );
    void  send_reaction_one( JsonDocument& doc );
    void  send_reaction_two( JsonDocument& doc );
    void send_flexion( JsonDocument& doc );
    void send_st_mode( JsonDocument& doc );
    
    
};

#endif // CONFIG_H
