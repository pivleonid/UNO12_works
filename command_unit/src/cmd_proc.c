/* Sep 16, 2015 */

/*!  \file 	fsm_protocol_cdc.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include <ctype.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "cmd_proc.h"
#include "inttask_messages.h"
#include "protocol_data.h"
#include "hrdwr_func.h"

/*defines===========================================================================================================*/

/*types=============================================================================================================*/


/*prototypes========================================================================================================*/
static err_code_t  set_freq     ( void*, void*);
static err_code_t  get_cu_sts   ( void*, void* );

static err_code_t  set_code     ( void*, void* );
static err_code_t  set_dac      ( void*, void* );
static err_code_t  set_dds      ( void*, void* );

static err_code_t  set_mechatt  ( void*, void* );
static err_code_t  set_pres     ( void*, void* );
static err_code_t  get_pres     ( void*, void* );
        
static err_code_t  set_yig      ( void*, void* );
static err_code_t  get_yig      ( void*, void* );
                
static err_code_t  set_ifamp    ( void*, void* );
static err_code_t  set_if       ( void*, void* );
static err_code_t  get_ifamp    ( void*, void* );
static err_code_t  get_if       ( void*, void* );
        
static err_code_t  set_uno      ( void*, void* );
static err_code_t  get_uno      ( void*, void* );
                
static err_code_t  echo_set     ( void*, void* ); 

cmd_errcode_t _message_reaction( cmd_code_t	cmdcode, err_code_t *opcode, void* data, void* answ_data, uint32_t* index );
cmd_errcode_t _answer_send( adress_code_t   source_addr, err_code_t opcode, void* data_out, uint32_t index );

/*variables=========================================================================================================*/
const struct _CMD_TABLE
    {
    cmd_code_t	    code;
    char*		    string;
    uint8_t         arg_num;
    err_code_t      ( *react_func )( void*, void* );
    } cmd_tbl[] = { 
    {   CMDCODE_SETCODE,        "setcode",  2,  set_code    },
    {   CMDCODE_SETDAC,         "setdac",   2,  set_dac     },
    {   CMDCODE_DDS,            "setdds",   3,  set_dds     },

    {   CMDCODE_ATT,            "mechatt",  1,  set_mechatt },
    {   CMDCODE_PRES,           "pres",     4,  set_pres    },
    {   CMDCODE_GET_PRES_STS,   "presstat", 0,  get_pres    },
        
    {   CMDCODE_YIG,            "yig",      1,  set_yig     },
    {   CMDCODE_GET_YIG,        "getyig",   0,  get_yig     },
                
    {   CMDCODE_SET_ATT0,       "setifamp", 3,  set_ifamp   },
    {   CMDCODE_SET_IF,         "setif",    5,  set_if      },
    {   CMDCODE_GET_ATT0,       "getifamp", 0,  get_ifamp   },
    {   CMDCODE_GET_IFSTS,      "getif",    0,  get_if      },
        
    {   CMDCODE_UNO,            "setuno",   4,  set_uno     },
    {   CMDCODE_GETUNO,         "getuno",   0,  get_uno     },
                
    {   CDMCODE_GET_STATUS,     "geta",  0,  get_cu_sts  },
    {   CMDCODE_FREQ,           "freq",     1,  set_freq    },
    {   0xFE,                   "echo",     1,  echo_set    },        
    { 0, "\0", 0, NULL },
    };


typedef struct DAC_FREQ_TABLE
    {
    uint16_t    dac[8];
    }dac_freq_table_t; 


/*code==============================================================================================================*/

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/

PT_THREAD( pt_cmd_proc
          ( 
           struct pt *pt              /* (вх) указатель на структуру состояния потока   */         
           ) )
    {
    cmd_code_t	    code;
    void*           data;
    adress_code_t   source_addr;
    uint32_t        ind;
    err_code_t      opcode;
    
    PT_BEGIN( pt );
		    
    PT_WAIT_UNTIL( pt, recv_message( CMD_FSM, &opcode, &code, &source_addr, &data ) == OK );
           
    if ( _message_reaction( code, &opcode, data, cmd_data_out, &ind ) == CMD_ERRCODE_OK ) {
            _answer_send( source_addr, opcode, cmd_data_out, ind );
        }
    
	    
    PT_END( pt );
	
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
cmd_errcode_t _message_reaction( cmd_code_t	cmdcode, err_code_t *opcode, void*  data, void* answ_data, uint32_t* index )
    {
    uint32_t 	i;
	
    if ( cmdcode == 0 ) {
        *opcode = _RESULT_NOOP;
        return CMD_ERRCODE_OK;
        }
    
    for ( i = 0; i < sizeof cmd_tbl / sizeof cmd_tbl[0]; i++ ) {
        if ( cmd_tbl[i].code == cmdcode ) {
        if ( cmd_tbl[i].react_func != NULL )
            {
            *opcode = cmd_tbl[i].react_func( data, answ_data );	
            }
        else
            {
            *opcode = _RESULT_OK;
            }
            *index = i;
            break;
            }
        }
    
    if ( i == sizeof cmd_tbl / sizeof cmd_tbl[0] ) {
        *opcode = _RESULT_NOOP;
        return CMD_ERRCODE_FAIL;
        }
    
    return CMD_ERRCODE_OK;
    }
	

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
cmd_errcode_t   _answer_send( adress_code_t  source_addr, err_code_t opcode, void* data_out, uint32_t index )
    {
    switch( source_addr )
        {
        case CDCRECV_FSM:   send_message( cmd_tbl[index].code, opcode, CDCSEND_FSM, CMD_FSM, data_out ); break;
        default:            return CMD_ERRCODE_FAIL;
        }
    
    return CMD_ERRCODE_OK;
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова установка настройки на частоту 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
err_code_t  set_freq( void* in_data, void* answ_data)
    {
    uint16_t    freq_value;
    
    //SET_FREQ(freq_value)
    /* big endian */
//    freq_value = ((uint8_t*)in_data)[0] << 8;
//    freq_value |= ((uint8_t*)in_data)[1] << 0;
//    
    freq_value = 123;
    if ( freq_value > 4301 )
        {
        return _RESULT_BADVALUE;        
        }
    
    return _RESULT_OK;
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
err_code_t  get_cu_sts( void* in_data, void* answ_data )
    {
    custs_flags_t   temp_sts_flags;
    uint16_t        dac_codes;
    
    
    /* статус БУ */
    temp_sts_flags.bitflags.alarm = 0;
    temp_sts_flags.bitflags.alarm_code = ALARM_ALL;
    temp_sts_flags.bitflags.type = CU_TYPE_HETERODIN;
    ((uint8_t*)answ_data)[0] = temp_sts_flags.byteflags;
    
    /* температура */
    ((uint8_t*)answ_data)[1] = 0;
    
    /* ЦАП0 */
    dac_codes = 0x00F;
    memcpy( &((uint8_t*)answ_data)[2], &dac_codes, 2 );
    
    /* ЦАП1 */
    dac_codes = 0x010;
    memcpy( &((uint8_t*)answ_data)[4], &dac_codes, 2 );
    
    /* ЦАП2 */
    dac_codes = 0x011;
    memcpy( &((uint8_t*)answ_data)[6], &dac_codes, 2 );

    /* ЦАП3 */
    dac_codes = 0x012;
    memcpy( &((uint8_t*)answ_data)[8], &dac_codes, 2 );

    /* ЦАП4 */
    dac_codes = 0x013;
    memcpy( &((uint8_t*)answ_data)[10], &dac_codes, 2 );
    
    /* ЦАП5 */
    dac_codes = 0x014;
    memcpy( &((uint8_t*)answ_data)[12], &dac_codes, 2 );
    
    /* ЦАП6 */
    dac_codes = 0x015;
    memcpy( &((uint8_t*)answ_data)[14], &dac_codes, 2 );
    
    /* ЦАП7 */
    dac_codes = 0x016;
    memcpy( &((uint8_t*)answ_data)[16], &dac_codes, 2 );

    /* ЦАП8 */
    dac_codes = 0x018;
    memcpy( &((uint8_t*)answ_data)[18], &dac_codes, 2 );
    
    /* АЦП0 */
    dac_codes = 0x020;
    memcpy( &((uint8_t*)answ_data)[20], &dac_codes, 2 );

    /* АЦП1 */
    dac_codes = 0x022;
    memcpy( &((uint8_t*)answ_data)[22], &dac_codes, 2 );
    
    
    return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_code( void* in_data, void* answ_data )
    {
        set_decoder_code( ((uint8_t*)in_data)[0], ((uint8_t*)in_data)[1] );

        return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_dac( void* in_data, void* answ_data )
    {
        uint16_t    code_value;
    
        memcpy( &code_value, ((uint8_t*)in_data)[1], 2 );
        set_dac_code( ((uint8_t*)in_data)[0], code_value );
    
        return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_dds( void* in_data, void* answ_data )
    {
        uint32_t    freq;
    
        memcpy( &freq, ((uint8_t*)in_data)[1], 4 );
        set_dds_code(((uint8_t*)in_data)[0], freq, ((uint8_t*)in_data)[5]);
        
        return _RESULT_OK;        
    }
        

/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_mechatt( void* in_data, void* answ_data )
    {
    set_mechatt_gain( ((uint8_t*)in_data)[0] );
    
    return _RESULT_OK;        
    }
        
        
/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_pres( void* in_data, void* answ_data )
    {    
    set_mechatt_flags( ((uint8_t*)in_data)[0] );
    
    return _RESULT_OK;        
    }
        
/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  get_pres( void* in_data, void* answ_data )
    {
    att_flags_t  *temp_flags = get_mechatt_flags();
    
    ((uint8_t*)answ_data)[0] = get_mechatt_gain();
    ((uint8_t*)answ_data)[1] = (uint8_t)(temp_flags);
    return _RESULT_OK;        
    }
        
        
/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_yig( void* in_data, void* answ_data )
    {
    set_yig_sts( ((uint8_t*)in_data)[0] );
    return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  get_yig( void* in_data, void* answ_data )
    {
    ((uint8_t*)answ_data)[0] = get_yig_sts();    
    return _RESULT_OK;        
    }

/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_ifamp( void* in_data, void* answ_data )
    {
    return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_if( void* in_data, void* answ_data )
    {
    
    return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  get_ifamp( void* in_data, void* answ_data )
    {
    
    return _RESULT_OK;        
    }

/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  get_if( void* in_data, void* answ_data )
    {
    
    return _RESULT_OK;        
    }


/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  set_uno( void* in_data, void* answ_data )
    {
    
    return _RESULT_OK;        
    }

/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  get_uno( void* in_data, void* answ_data )
    {
    
    return _RESULT_OK;        
    }
                
/*=============================================================================================================*/
/*! \brief   функция обратного вызова 

    \return признак завершения команды
    \retval _RESULT_OK, _RESULT_BADVALUE
*/
/*=============================================================================================================*/
static err_code_t  echo_set( void* in_data, void* answ_data )
    {
    
    return _RESULT_OK;        
    }








            /*=============================================================================================================*/
            /*!  \brief

                         \return 
                         \retval 
                         \sa 
                         */
                         /*=============================================================================================================*/
uint8_t get_code_from_string( char* code_string )
    {
    uint32_t	i;
	
    for ( i = 0; cmd_tbl[i].code != 0; i++ ) {
        if ( strcmp( code_string, cmd_tbl[i].string ) == 0 ) {
            break;
            }		
        }
	
    return cmd_tbl[i].code;
    }


            /*=============================================================================================================*/
            /*!  \brief

                         \return 
                         \retval 
                         \sa 
                         */
                         /*=============================================================================================================*/
int get_argnumfromcode( uint8_t code )
    {
    uint32_t    i;
    
    if ( code == 0 ) {
            return 0;
        }
    
    for ( i = 0; cmd_tbl[i].code != 0; i++ ) {
        if ( code == cmd_tbl[i].code ) {
            break;
            }		
        }
	
    return cmd_tbl[i].arg_num;
    
    }


            /*=============================================================================================================*/
            /*!  \brief

                         \return 
                         \retval 
                         \sa 
                         */
                         /*=============================================================================================================*/
uint8_t	trans_data_from_string
(
 uint8_t    cmdcode, 
 char*	    string,
 char**     end, 
 void*      output_buf,
 size_t*     outlen
 )
    {
    char *sym = string;
    
    while ( *sym != 0x00 )
        {
        if ( *sym != 0x20 && !isalnum( *sym ) )
            {
                return ERROR;
            }
        sym++;
        } 
    
    switch ( cmdcode ) {            
            case CMDCODE_FREQ:
                {
                uint16_t freq;
                *sym = string;
                    
                freq = strtol( string, end, 16 );
                if ( *end == NULL ) {
                    return ERROR;                    
                    }
                memcpy( &((uint8_t*)output_buf)[0], &freq, 2 );
                if ( outlen != NULL ) {
                    *outlen = 2;
                    }                                
                }
            break;
                        
            case CMDCODE_YIG: 
                {
                    uint8_t code;
                    *sym = string;
                            
                    code = strtol( string, end, 10 );
                    ((uint8_t*)output_buf)[0] = (code >> 0) & 0xFF;
                
                    while ( (code == 0) && (*sym != 0x00) ) {
                        if ( !isdigit( *sym++ ) ) {
                            return ERROR;
                        }
                    } 
                    if ( outlen != NULL ) {
                        *outlen = 1;
                    }                
                }
            break;
            
            
            case CMDCODE_SETCODE: 
            case CMDCODE_SETDAC: 
                {
                    uint8_t id;
                    uint8_t code;
                    
                    *sym = string;
                    id = strtol( string, end, 16 );
                    if ( *end == NULL ) {
                        return ERROR;                    
                    }
                    code =  strtol( *end, end, 16 );
                    ((uint8_t*)output_buf)[0] = id;
                    memcpy( &((uint8_t*)output_buf)[1], &code, 2 );
                    if ( outlen != NULL ) {
                        *outlen = 3;
                    }                                
                }
            break;
            
            case CMDCODE_DDS: 
                {
                    uint8_t     id;
                    uint32_t    freq;
                    uint16_t    gain;
                
                    *sym = string;
                    id = strtol( string, end, 16 );
                    if ( *end == NULL ) {
                        return ERROR;                    
                    }
                    
                    freq = strtol( string, end, 10 );
                    if ( *end == NULL ) {
                        return ERROR;                    
                    }

                    gain = strtol( string, end, 10 );
                    if ( *end == NULL ) {
                        return ERROR;                    
                    }
                
                    ((uint8_t*)output_buf)[0] = id;
                    memcpy(&((uint8_t*)output_buf)[1], &freq, 4);
                    ((uint8_t*)output_buf)[5] = (uint8_t)(gain / 5);
                
                    if ( outlen != NULL ) {
                        *outlen = 6;
                    }                                               
                }
            break;
            
            case CMDCODE_ATT: 
                {
                    int32_t chslo;
                
                    *sym = string;
                        
                    chslo = strtol( string, end, 10 );
                    ((uint8_t*)output_buf)[0] = (chslo >> 0) & 0xFF;
                
                    while ( (chslo == 0) && (*sym != 0x00) ) {
                        if ( !isdigit( *sym++ ) ) {
                            return ERROR;
                            }
                        } 
                    if ( outlen != NULL ) {
                        *outlen = 1;
                        }                
                }
            break;
            
            case CMDCODE_PRES: 
                {
                att_flags_t  att_flags;
                uint8_t      no_parse = 0;
                
                if ( strstr( string, " lnaoff" ) )
                    {
                    no_parse++;
                    att_flags.bitflags.lna = 0;
                    }
                else if ( strstr( string, " lnaon" ) )
                    {
                    no_parse++;
                    att_flags.bitflags.lna = 1;                    
                    }
                
                if ( strstr( string, " plnaoff" ) ) {
                    no_parse++;
                    att_flags.bitflags.preslna = 0;
                    }
                else if ( strstr( string, " plnaon" ) ) {
                no_parse++;
                att_flags.bitflags.preslna = 1;                    
                    }
                
                if ( strstr( string, " tstoff" ) ) {
                    no_parse++;
                    att_flags.bitflags.test = 0;
                    }
                else if ( strstr( string, " tston" ) ) {
                no_parse++;
                att_flags.bitflags.test = 1;                    
                    }

                if ( strstr( string, "off " ) ) {
                    no_parse++;
                    att_flags.bitflags.on = 0;
                    }
                else if ( strstr( string, "on " ) ) {
                no_parse++;
                att_flags.bitflags.on = 1;                    
                    }
                
                *end = string + strlen( string );
                if ( no_parse < 4 )
                    {
                        return ERROR;                        
                    }

                }
            break;

        case CMDCODE_SET_ATT0:
            {
            
            }
            break;
            
        case CMDCODE_SET_IF:
            {
            
            }
            break;
            
        case CMDCODE_UNO:
            {
            
            }
            break;
            
        default:
                {
                *end = string;
                if ( outlen != NULL ) {
                    *outlen = 0;
                    }                
                }
            break;
            
            
    }	
    
    return OK;
    }




