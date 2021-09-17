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

#define FIRMWARE_VERSION                "2.0.08"  // Se agrego fuerza minima a aplicar FORCE_MIN. 
                                                  // Se cambio nombre , de flexion a tof_flexion para identificar las distintas fuentes de medicion para la flexion. 
                                                  // TODO:Cuando se modifica el cliente, quedan pendiente modificar los jSOn ejemplo {info:'flexion'} pasara a {info:'tof_flexion'
                                                  // Se agrego parametro step_flexion para devolver el calculo de la flexion medido con los pasos del motor 2.
                                                  // Se agrego calculo de flexion en milimitros con:
                                                  // K_CONVERTER_STEP_MM                     0.000541   // Constante de conversion de pasos a milimetro de flexion.
                                                  // Se agrego macro de precompilacion CELL_FORCE_EQUAL_REACTIONS para realimentar la fuerza aplicada con la suma de las reacciones.

//#define EEPROM_ADDRESS_CONFIG         4       // Direccion en la epprom donde se almacena la configuracion.
#define MAGIC_NUMBER                    19    // Numero magico para detectar memoria sin inicializar.


#define DISTANCE_DEFAULT                399            // Distancia por defecto donde se aplica la fuerza 100 mm.
#define FORCE_DEFAULT                   10000          // Fuerza por defecto a aplicar Gramos.
#define REACTION1_DEFAULT               0              //  Fuerza de reaccion 1 por defecto.
#define REACTION2_DEFAULT               0              //  Fuerza de reaccion 2 por defecto.
#define TOF_FLEXION_DEFAULT             0              //  Flexion por defecto del tof. 
#define STEP_FLEXION_DEFAULT            2.123456       //   Flexion por defecto del los pasos(6 decimales maximo)
#define ST_TEST_DEFAULT                 0              //  Estado del test pòr defecto.
#define ST_MODE_DEFAULT                 ST_MODE_TEST   //  Modo de operacion del sistema. 

                                                                                      

#define FORCE_MAX                       5000           // Fuerza maxima aplicada en el experimento. 
#define FORCE_MIN                       1000           // Fuerza minima aplicada en el experimento.

// Mapa de direcciones de los campos de configuracion en la EEPROM.
#define EEPROM_ADDRESS_MAGIC_NUMBER     0
#define EEPROM_ADDRESS_DISTANCE        (EEPROM_ADDRESS_MAGIC_NUMBER + sizeof(uint8_t))
#define EEPROM_ADDRESS_FORCE           (EEPROM_ADDRESS_DISTANCE + sizeof(uint16_t))
#define EEPROM_ADDRESS_REACTION_1      (EEPROM_ADDRESS_FORCE + sizeof(float))
#define EEPROM_ADDRESS_REACTION_2      (EEPROM_ADDRESS_REACTION_1 + sizeof(float))
#define EEPROM_ADDRESS_TOF_FLEXION         (EEPROM_ADDRESS_REACTION_2 + sizeof(float))
#define EEPROM_ADDRESS_LOG_LEVEL       (EEPROM_ADDRESS_TOF_FLEXION + sizeof(uint8_t))
#define EEPROM_ADDRESS_ST_TEST         (EEPROM_ADDRESS_LOG_LEVEL + sizeof(uint8_t))  
#define EEPROM_ADDRESS_ST_MODE        (EEPROM_ADDRESS_ST_TEST + sizeof(uint8_t))  
#define EEPROM_ADDRESS_STEP_FLEXION   (EEPROM_ADDRESS_ST_MODE + sizeof(uint8_t))  //este valor es  float 

/*
 *  Para poder leer los dispositivo y ejecutar accciones se pone un modo de operacion .
 *  
 */


#define ST_MODE_TEST                    0               // Modo de operacion normal, ensayo activado.
#define ST_MODE_TOF                     1               // Modo de operacion TOF, muestra Tof sin promedio.
#define ST_MODE_TOF_AVERAGE             2               // Modo de operacion TOF, muestra Tof con promedio
#define ST_MODE_HOME_M2                 3               // Va al home del motor 2.
#define ST_MODE_CELL                    4               // Muestra el valor de las celdas.


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

    uint8_t get_tof_flexion( void );
    void set_tof_flexion( uint8_t ); 

    float get_step_flexion( void );
    void set_step_flexion( float );     

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


    uint16_t distance;         // Distancia donde se aplica la fuerza.
    float force;              // Fuerza a aplicar.
    float reaction1;          // Fuerza de reaccion 1.
    float reaction2;          // Fuerza de reaccion 2.
    uint8_t tof_flexion;      // Distancia de flexion del tof.
    float step_flexion;      // Distancia de flexion de contar los step.
    
    void send_all_params( JsonDocument& );
    void send_version( JsonDocument& );
    void send_ok( JsonDocument& );
    void send_ack( JsonDocument& );
    void send_status( JsonDocument& doc );
    void  send_reaction_one( JsonDocument& doc );
    void  send_reaction_two( JsonDocument& doc );
    void send_tof_flexion( JsonDocument& doc );
    void send_step_flexion( JsonDocument& doc );
    void send_st_mode( JsonDocument& doc );
    
    
};

#endif // CONFIG_H
