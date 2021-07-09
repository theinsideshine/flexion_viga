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
#include <ArduinoJson.h>

#define FIRMWARE_VERSION                "1.0.14"  // New Version 1.0.14 Se cambio pin 13 al 12(pul_mtr1), para evitar oscilaciones en la carga del firmware y reset.
                                                  // added speed macros comments for motors 1 and 2 for 200 steps per turn
                                                  // add pin strange gauge for undef TEST_PROTOTIPE
                                                  
        
//#define EEPROM_ADDRESS_CONFIG         4       // Direccion en la epprom donde se almacena la configuracion.
#define MAGIC_NUMBER                    23     // Numero magico para detectar memoria desinicializada.


#define DISTANCE_DEFAULT                23     // Distancia por defecto donde se aplica la fuerza 25 cm.
#define FORCE_DEFAULT                   306     // Fuerza por defecto a aplicar Gramos.
#define REACTION1_DEFAULT               0     //  Fuerza de reaccion 1 por defecto.
#define REACTION2_DEFAULT               0     //  Fuerza de reaccion 2 por defecto.
#define FLEXION_DEFAULT                 0     //  Flexion por defecto. 

#define ST_TEST_DEFAULT                 0     //  Estado del test pòr defecto.


// Mapa de direcciones de los campos de configuracion en la EEPROM.
#define EEPROM_ADDRESS_MAGIC_NUMBER     0
#define EEPROM_ADDRESS_DISTANCE        (EEPROM_ADDRESS_MAGIC_NUMBER + sizeof(uint8_t))
#define EEPROM_ADDRESS_FORCE           (EEPROM_ADDRESS_DISTANCE + sizeof(uint16_t))
#define EEPROM_ADDRESS_REACTION_1      (EEPROM_ADDRESS_FORCE + sizeof(uint16_t))
#define EEPROM_ADDRESS_REACTION_2      (EEPROM_ADDRESS_REACTION_1 + sizeof(float))
#define EEPROM_ADDRESS_FLEXION         (EEPROM_ADDRESS_REACTION_2 + sizeof(float))
#define EEPROM_ADDRESS_LOG_LEVEL       (EEPROM_ADDRESS_FLEXION + sizeof(uint8_t))
#define EEPROM_ADDRESS_ST_TEST         (EEPROM_ADDRESS_LOG_LEVEL + sizeof(uint8_t))  //este valor es deberia es uint_8 


class CConfig
{
  public:
    CConfig();
    uint16_t get_distance( void );
    void set_distance( uint16_t );
    
    uint16_t get_force( void );
    void set_force( uint16_t ); 

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
 

    void host_cmd( void );
    void send_test_finish(void);

  private:
    uint8_t log_level;          // 0 = log de informacion de control desactivada.
    uint8_t st_test;            // Estado del ensayo 

    uint16_t distance;            // Distancia donde se aplica la fuerza.
    uint16_t force;              // Fuerza a aplicar.
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
    
    
};

#endif // CONFIG_H
