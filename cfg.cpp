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
 
#include "cfg.h"
#include "log.h"
#include "motor.h"

#include <EEPROM.h>


CMotor   Motor_host;

CConfig::CConfig()
{
uint8_t magic_number;

    EEPROM.get( EEPROM_ADDRESS_MAGIC_NUMBER, magic_number );

    if( magic_number != MAGIC_NUMBER ){
        magic_number = MAGIC_NUMBER;
        EEPROM.put( EEPROM_ADDRESS_MAGIC_NUMBER, magic_number );

        set_distance( DISTANCE_DEFAULT );
        set_force( FORCE_DEFAULT );
        set_reaction1(REACTION1_DEFAULT );
        set_reaction2(REACTION2_DEFAULT );
        set_flexion(FLEXION_DEFAULT); 

        //set_log_level( LOG_DISABLED );
        set_log_level( LOG_MSG ); // cuando cambias el magic numbre setea los mensajes de logeo (Arduino 1,primer inicio")        
        set_st_test(ST_TEST_DEFAULT);

        
    }else {
        EEPROM.get( EEPROM_ADDRESS_DISTANCE, distance );
        EEPROM.get( EEPROM_ADDRESS_FORCE, force);
        EEPROM.get( EEPROM_ADDRESS_REACTION_1, reaction1 );
        EEPROM.get( EEPROM_ADDRESS_REACTION_2, reaction2 );
        EEPROM.get( EEPROM_ADDRESS_FLEXION, flexion );
        
        EEPROM.get( EEPROM_ADDRESS_LOG_LEVEL, log_level );
       //EEPROM.get( EEPROM_ADDRESS_ST_TEST, st_test );
       //este parametro inicializa en 0 independiente de como este en la eprrom
       set_st_test(ST_TEST_DEFAULT);
        
    }
}

uint16_t CConfig::get_distance( void )
{
    return distance;
}

void CConfig::set_distance( uint16_t val )
{
    distance = val;
    EEPROM.put( EEPROM_ADDRESS_DISTANCE, val );
}

uint16_t CConfig::get_force( void )
{
    return force;
}

void CConfig::set_force( uint16_t val )
{
    force = val;
    EEPROM.put( EEPROM_ADDRESS_FORCE, val );
}

float CConfig::get_reaction1( void )
{
    return reaction1;
}

void CConfig::set_reaction1( float val )
{
   
    reaction1 = val;
    EEPROM.put( EEPROM_ADDRESS_REACTION_1, val );
}

float CConfig::get_reaction2( void )
{
    return reaction2;
}

void CConfig::set_reaction2( float val )
{
    reaction2 = val;
    EEPROM.put( EEPROM_ADDRESS_REACTION_2, val );
}

uint8_t CConfig::get_flexion( void )
{
    return flexion;
}

void CConfig::set_flexion( uint8_t val )
{
    flexion = val;
    EEPROM.put( EEPROM_ADDRESS_FLEXION, val );
}


uint8_t CConfig::get_log_level( void )
{
    return log_level;
}

void CConfig::set_log_level( uint8_t enable )
{
    log_level = enable;
    EEPROM.put( EEPROM_ADDRESS_LOG_LEVEL, log_level );
}
/*
 * Estos deberia ser booleano !!!!!
 * 
 */
 
uint8_t CConfig::get_st_test( void )
{
    return st_test;
}

void CConfig::set_st_test( uint8_t enable )
{
    st_test = enable;
    EEPROM.put( EEPROM_ADDRESS_ST_TEST, st_test );
}



// Lee por el puerto serie parametros de configuracion en formato json.
// {info:'all-params'}   Envia todos los parametros en formato json.
// {info:'version'}      Envia  la version del firmware.
// {info:'status'}       Devuelve el estatus del ensayo.
// {info:'reaction_one'}    Devuelve la reaction1 del ensayo.
// {info:'reaction_two'}    Devuelve la reaction2 del ensayo.
// {info:'flexion'}      Devuelve la flexion del ensayo.



// {log_level:'0'}       log_level:0=desactivado,
// {log_level:'1'}                 1=mensajes.
// {log_level:'2'}                 2=info control estandar.
// {log_level:'3'}                 3=info control arduino plotter.

// {cmd:'start'}   Comienza el ensayo.
// {m1f:'50'}       Mueve 6 mm el motor 1 hacia adelante.
// {m1r:'4'}       Mueve 4 mm el motor 1 hacia atras.



 //   {cdd:'start',data:{distance:'20',force:'306'}} 


//TODO: En futuro debe admitir decimales 

// {distance:'500'}      distance:0 a 254       Distancia en mm donde se aplica la fuerza.
// {force:'11'}          force:0 a 254          Fuerza a aplicar en Kg.
// {reaction_one:'1'}    reaction_one :0 a 254  Fuerza de reaccion uno, en Kg.
// {reaction_two:'2'}    reaction_two :0 a 254  Fuerza de reaccion dos, en Kg.
// {flexion:'3'}         flexion :0 a 254       Flexion del ensayo, en mm.
// {st_test:'1'}         st_test : 0 ensayo desactivado. 
//                       st_test : 1 ensayo activado. 


void CConfig::host_cmd( void )
{
bool known_key = false;

    if ( Serial.available() ){
        StaticJsonDocument<512> doc;
        auto error = deserializeJson( doc, Serial );
        if ( !error ) {
          
            if ( doc.containsKey("distance") ) {
                set_distance( doc["distance"] );
                known_key = true;
            }

            if ( doc.containsKey("force") ) {
                set_force( doc["force"] );
                known_key = true;
            }

            if ( doc.containsKey("reaction_one") ) {
                set_reaction1( doc["reaction_one"] );
                known_key = true;
            }

            if ( doc.containsKey("reaction_two") ) {
                set_reaction2( doc["reaction_two"] );
                known_key = true;
            }

            if ( doc.containsKey("flexion") ) {
                set_flexion( doc["flexion"] );
                known_key = true;
            }

            /*             
             *              
            if ( doc.containsKey("st_test") ) {
                set_st_test( doc["st_test"] );
                known_key = true;
            }
            */ 
            
            if ( doc.containsKey("log_level") ) {
                set_log_level( doc["log_level"] );
                known_key = true;
            }                
                   

            if ( doc.containsKey("info") ) {
                String key = doc["info"];

                if( key == "all-params" ) {
                    send_all_params( doc );
                }else if( key == "version" ) {
                    send_version( doc );
                }else if( key == "status" ) {
                    send_status( doc );
                }else if( key == "reaction_one" ) {
                    send_reaction_one( doc );
                }else if( key == "reaction_two" ) {
                    send_reaction_two( doc );
                }else if( key == "flexion" ) {
                    send_flexion( doc );
                }
                
            }

            // Usamos cdd hasta definir que unifiquemos comandos, en realidad es cmd. 
            // Segun el uso https://arduinojson.org/v6/api/jsondocument/containskey/ 
            // para preguntar sobre la clave de un objeto  se usa  const char* key1 =doc["data"]["distance"];

            
            if ( doc.containsKey("cdd") ) { //Verifica "cdd".
                String key = doc["cdd"];
                 if( key == "start" ) {    //Verifica "start".
                     const char* key1 =doc["data"]["distance"];
                     const char* key2 =doc["data"]["force"];
                    if (key1 && key2) {   //Verifica "distance". "force".
                       set_distance( doc["data"]["distance"]);
                       set_force( doc["data"]["force"] );  
                       set_st_test( 1 );  // Comienza el ensayo.                      
                       send_ack( doc );
                     }
                  
                 }
             }           

             if ( doc.containsKey("m1f") ) {                                            
                    
                    Motor_host.fwd_m1(doc["m1f"]);                
                    send_ack( doc );                          
            } 
             if ( doc.containsKey("m1r") ) {
                
                    Motor_host.rwd_m1(doc["m1r"]);                
                    send_ack( doc );                          
            } 


        
            if ( doc.containsKey("cmd") ) {
                String key = doc["cmd"];                
                
                if( key == "start" ) {
                    set_st_test( 1 );  // Comienza el ensayo.
                    send_ack( doc );
                }          
            } else if( known_key == true ) {
                send_ok( doc );
            }

            
        
        
        }
    }
}

// Envia todos los parametros de configuracion en formato json.
// Mas adelante, se podria usar el parametro del comando rea
// para leer un parametro en especial.
void CConfig::send_all_params( JsonDocument& doc )
{
    doc["distance"] = get_distance();
    doc["force"] =  get_force();
    doc["reaction_one"] = get_reaction1();
    doc["reaction_two"] =  get_reaction2();
    doc["flexion"] = get_flexion();     
    doc["log_level"] = get_log_level();
    doc["st_test"] = get_st_test();  
   
    serializeJsonPretty( doc, Serial );
}

//Envia el status del test
void CConfig::send_test_finish( void )
{
    StaticJsonDocument<512> doc;   
    doc["st_test"] = get_st_test();  
    serializeJsonPretty( doc, Serial );
}



// Envia la version del firmware.
void CConfig::send_version( JsonDocument& doc )
{
    doc["version"] = FIRMWARE_VERSION;

    serializeJsonPretty( doc, Serial );
}

// Envia la reaccion 1
void CConfig::send_reaction_one( JsonDocument& doc )
{
    doc["reaction_one"] =  get_reaction1();;

    serializeJsonPretty( doc, Serial );
}

// Envia la reaccion 2
void CConfig::send_reaction_two( JsonDocument& doc )
{
    doc["reaction_two"] =  get_reaction2();;

    serializeJsonPretty( doc, Serial );
}


// Envia la flexion
void CConfig::send_flexion( JsonDocument& doc )
{
    doc["flexion"] =  get_flexion();;

    serializeJsonPretty( doc, Serial );
}

// Envia el estatus del ensayo.
void CConfig::send_status( JsonDocument& doc )
{
    doc["status"] = get_st_test();;

    serializeJsonPretty( doc, Serial );
}

// Envia el resultado en formato json
void CConfig::send_ok( JsonDocument& doc )
{
    doc[ "result" ] = "ok";

    serializeJsonPretty( doc, Serial );
}

// Envia el resultado en formato json
void CConfig::send_ack( JsonDocument& doc )
{
    doc[ "result" ] = "ack";

    serializeJsonPretty( doc, Serial );
}
