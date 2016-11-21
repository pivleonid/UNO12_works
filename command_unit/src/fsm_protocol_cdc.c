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
#include "fsm_protocol_cdc.h"
#include "cmd_proc.h"
#include "inttask_messages.h"
#include "protocol_data.h"
#include "hrdwr_func.h"

/*defines===========================================================================================================*/
#define CDC_INPUT_BUFSIZE	(128)
#define CDC_OUTPUT_BUFSIZE	(128)


/*types=============================================================================================================*/

enum CDC_SIGNAL {
    CDC_READY   = 0,
    CDC_CODE,
    CDC_DATA,
    CDC_CODEEND,
    CDC_DATAEND,
    CDC_END,
    };


enum CMD_SIGNAL {
    CMD_NULL = 0,
    CMD_FREQ,
    CMD_,
    };


/*prototypes========================================================================================================*/

int VCP_read( void *pBuffer, int size );
int VCP_write( const void *pBuffer, int size );
int VCP_get_size( void );

static void cdc_code_parse( int state, int signal );
static void cdc_data_save( int state, int signal );
static void cdc_start_cmdproc( int state, int signal );
static void cdc_reset( int state, int signal );

static void cdc_send_data( int state, int signal );
static void cdc_send_code( int state, int signal );

/*variables=========================================================================================================*/
const  fsm_table_t   cdc_recv_table[3][6] = {
    [CDCRECV_STATE_READY][CDC_READY]            = { CDCRECV_STATE_READY,        NULL },
    [CDCRECV_STATE_READY][CDC_CODE]             = { CDCRECV_STATE_CODEPARSE,    cdc_code_parse },
    [CDCRECV_STATE_READY][CDC_DATA]             = { CDCRECV_STATE_DATASAVE,     NULL },
    [CDCRECV_STATE_READY][CDC_CODEEND]          = { CDCRECV_STATE_CODEPARSE,    cdc_code_parse },
    [CDCRECV_STATE_READY][CDC_DATAEND]          = { CDCRECV_STATE_DATASAVE,     NULL },
    [CDCRECV_STATE_READY][CDC_END]              = { CDCRECV_STATE_READY,        cdc_reset },
		
    [CDCRECV_STATE_CODEPARSE][CDC_READY]        = { CDCRECV_STATE_READY,        cdc_reset },
    [CDCRECV_STATE_CODEPARSE][CDC_CODE]         = { CDCRECV_STATE_CODEPARSE,    NULL },
    [CDCRECV_STATE_CODEPARSE][CDC_DATA]         = { CDCRECV_STATE_DATASAVE,     cdc_data_save },
    [CDCRECV_STATE_CODEPARSE][CDC_CODEEND]      = { CDCRECV_STATE_READY,        cdc_start_cmdproc },
    [CDCRECV_STATE_CODEPARSE][CDC_DATAEND]      = { CDCRECV_STATE_DATASAVE,     cdc_data_save },
    [CDCRECV_STATE_CODEPARSE][CDC_END]          = { CDCRECV_STATE_READY,        cdc_start_cmdproc },

    [CDCRECV_STATE_DATASAVE][CDC_READY]         = { CDCRECV_STATE_READY,        cdc_reset },
    [CDCRECV_STATE_DATASAVE][CDC_CODE]          = { CDCRECV_STATE_DATASAVE,     NULL },
    [CDCRECV_STATE_DATASAVE][CDC_DATA]          = { CDCRECV_STATE_DATASAVE,     cdc_data_save },
    [CDCRECV_STATE_DATASAVE][CDC_CODEEND]       = { CDCRECV_STATE_READY,        cdc_start_cmdproc },
    [CDCRECV_STATE_DATASAVE][CDC_DATAEND]       = { CDCRECV_STATE_READY,        cdc_start_cmdproc },
    [CDCRECV_STATE_DATASAVE][CDC_END]           = { CDCRECV_STATE_READY,        cdc_start_cmdproc },
    };

    	//static enum CDC_SIGNAL		cdc_runtime_signal	= { CDC_READY };
static enum CDCRECV_STATE   cdc_curnt_state = { CDCRECV_STATE_READY };
static enum CDC_SIGNAL  	cdc_curnt_signal = { CDC_READY };

const  fsm_table_t   cdc_send_table[3][3] = {
    [CDCSEND_STATE_READY][CDC_READY] = { CDCSEND_STATE_READY, NULL },
    [CDCSEND_STATE_READY][CDC_CODE] = { CDCSEND_STATE_CODEOFOP, cdc_send_code },
    [CDCSEND_STATE_READY][CDC_DATA] = { CDCSEND_STATE_DATASEND, cdc_send_data },
        
    [CDCSEND_STATE_DATASEND][CDC_READY] = { CDCSEND_STATE_DATASEND, cdc_send_data },
    [CDCSEND_STATE_DATASEND][CDC_CODE] = { CDCSEND_STATE_CODEOFOP, cdc_send_code },
    [CDCSEND_STATE_DATASEND][CDC_DATA] = { CDCSEND_STATE_DATASEND, cdc_send_data },
        
    [CDCSEND_STATE_CODEOFOP][CDC_READY] = { CDCSEND_STATE_READY, NULL },
    [CDCSEND_STATE_CODEOFOP][CDC_CODE] = { CDCSEND_STATE_READY, cdc_send_code },
    [CDCSEND_STATE_CODEOFOP][CDC_DATA] = { CDCSEND_STATE_READY, NULL },  
    };

static enum CDCSEND_STATE   cdc_curnt_sendstate = { CDCSEND_STATE_READY };
static enum CDC_SIGNAL  	cdc_curnt_sendsignal = { CDC_READY };


static uint8_t				input_cdcbuffer[CDC_INPUT_BUFSIZE];
static uint32_t             input_index = 0;
static uint32_t             parse_index = 0;
static char*				brk_sym = NULL; 

static uint8_t			    output_cdcbuffer[CDC_OUTPUT_BUFSIZE];
static uint8_t              echo_need = 1;

static uint8_t				code_in;
static uint8_t              *answ_data;
static err_code_t           answ_code;
static int                  input_agr_num;

static cmd_code_t			answmsg_code;


/*code==============================================================================================================*/

/*=============================================================================================================*/
/*!  \brief

     \return i2c_errcode_t
     \retval MCUI2C_OK, MCUI2C_INITERR
     \sa 
*/
/*=============================================================================================================*/
enum CDCRECV_STATE*	get_curst_cdc (void)
    {
    return &cdc_curnt_state ;
    };


/*=============================================================================================================*/
/*!  \brief

     \return i2c_errcode_t
     \retval MCUI2C_OK, MCUI2C_INITERR
     \sa 
*/
/*=============================================================================================================*/
int	 get_cursig_cdc( void )
    {
    uint32_t read_need;
    uint32_t read_max;
    uint32_t read_size = 0;
		
    if ( brk_sym == NULL ) {
        read_need = VCP_get_size();
	
        if ( read_need <= 0 ) {	
            return (int)cdc_curnt_signal;	
            }
	
        read_max =  (CDC_INPUT_BUFSIZE - 1) - input_index;
        read_need = (read_need > read_max) ? read_max : read_need;
        read_size = VCP_read( &input_cdcbuffer[input_index], read_need );
        if ( echo_need ) {
            VCP_write( &input_cdcbuffer[input_index], read_size );    	
            }

        input_index += read_size;	
        input_cdcbuffer[input_index] = 0;
    	
        }
	
    if ( cdc_curnt_signal < CDC_END ) {
        brk_sym = strpbrk( (const char*)&input_cdcbuffer[parse_index+1], " \r" );				
        }
    else {
        return (int)cdc_curnt_signal;
        }
	
    if ( brk_sym == NULL ) {		
        if ( input_index == (CDC_INPUT_BUFSIZE - 1) ) {
            memset( input_cdcbuffer, 0, (CDC_INPUT_BUFSIZE - 1) );			
            input_index = 0;
            }		
        }
    else if ( *brk_sym == ' ' ) {
        switch ( cdc_curnt_state ) {
            case CDCRECV_STATE_READY:
                *brk_sym = 0; 
                return cdc_curnt_signal = CDC_CODE;
            
            case CDCRECV_STATE_CODEPARSE:   {
                    if ( input_agr_num ) 
                        { input_agr_num--; parse_index = brk_sym - (char*)input_cdcbuffer; return cdc_curnt_signal = CDC_CODE; } 
                    else
                        { *brk_sym = 0; return cdc_curnt_signal = CDC_DATA; } 
                };
            
            case CDCRECV_STATE_DATASAVE:	
                *brk_sym = 0; 
                return cdc_curnt_signal = CDC_DATA;
            
            default:						
                *brk_sym = 0; 
                return cdc_curnt_signal = CDC_END;
            }
        }
    else if ( *brk_sym == '\r' ) {
        *brk_sym = 0;
        switch ( cdc_curnt_state ) {
                case CDCRECV_STATE_READY:		return cdc_curnt_signal = CDC_CODEEND;
                case CDCRECV_STATE_CODEPARSE:   return cdc_curnt_signal = CDC_DATAEND;
                case CDCRECV_STATE_DATASAVE:	return cdc_curnt_signal = CDC_DATAEND;
                default:						return cdc_curnt_signal = CDC_END;
            }
		
        }
	
    return cdc_curnt_signal;
    }



    	/*=============================================================================================================*/
    	/*!  \brief

    		     \return i2c_errcode_t
    		     \retval MCUI2C_OK, MCUI2C_INITERR
    		     \sa 
    		     */
    		     /*=============================================================================================================*/
static void cdc_code_parse( int state, int signal )
    {
    uint8_t  code;
    char	 *string;
    size_t   move_col;

    	/* ïåðåâîä â íèæíèé ðåãèñòð */
    string = (char*)input_cdcbuffer;
    while ( string != brk_sym ) {
        char lower_ch = (char)tolower( (int)*string );
        *string++ = lower_ch;
        }
	
    code = get_code_from_string( (char*)input_cdcbuffer );
    move_col = ((int)brk_sym - (int)input_cdcbuffer + 1);
    input_index -= move_col;
//    memmove( input_cdcbuffer, brk_sym + 1, move_col );
	memmove(input_cdcbuffer, brk_sym + 1, input_index);    
    memset( &input_cdcbuffer[input_index], 0, move_col );
    	
    code_in = code;
    input_agr_num = get_argnumfromcode( code );
    parse_index = 0;
	    	    
    if ( code == 0 )
        {
        send_message( code_in, _RESULT_NOOP, CDCSEND_FSM, CDCRECV_FSM, cmd_data_out ); 
        cdc_curnt_signal = CDC_READY;
        }
    else if ( input_agr_num == 0 )
        {
        cdc_curnt_signal = CDC_CODEEND;        
        }
    }

    	/*=============================================================================================================*/
    	/*!  \brief

    		     \return i2c_errcode_t
    		     \retval MCUI2C_OK, MCUI2C_INITERR
    		     \sa 
    		     */
    		     /*=============================================================================================================*/
static void cdc_data_save( int state, int signal )
    {
    char	 *string;
    char     *end_of_ch; 
    size_t   datalen;
    size_t   move_col;
	
    /* ïåðåâîä â íèæíèé ðåãèñòð */
    string = (char*)input_cdcbuffer;
    while ( string != brk_sym ) {
        char lower_ch = (char)tolower( (int)*string );
        *string++ = lower_ch;
        }

//	    {
//		    uint8_t		buf[64];
//		    uint8_t		len;
//		            
//		    len = sprintf(buf, "\n\r%u %u\n\r", code, input_agr_num);
//		    VCP_write(buf, len);
//	    }
//
    if ( trans_data_from_string( code_in, (char*)input_cdcbuffer, &end_of_ch, cmd_data_in, &datalen ) != OK ) {
        send_message( code_in, _RESULT_BADFORMAT, CDCSEND_FSM, CDCRECV_FSM, cmd_data_out ); 
        cdc_curnt_signal = CDC_READY;
        end_of_ch = brk_sym;        
        }
//    else if ( signal == CDC_DATAEND ) {
//         cdc_curnt_signal = CDC_DATAEND;	        
//        }
//    
    cmd_data_index += datalen;
//    if ( end_of_ch != brk_sym )
//        {
//        send_message( code_in, _RESULT_BADFORMAT, CDCSEND_FSM, CDCRECV_FSM, cmd_data_out ); 
//        cdc_curnt_signal = CDC_READY;
//        end_of_ch = brk_sym;                
//        }
//    
    parse_index = 0;
    move_col = ((int)end_of_ch - (int)input_cdcbuffer + 1);
    input_index -= move_col;
    memmove( input_cdcbuffer, end_of_ch + 1, move_col );
    memset( &input_cdcbuffer[input_index], 0, move_col );
    }    


/*=============================================================================================================*/
/*!  \brief

     \return i2c_errcode_t
     \retval MCUI2C_OK, MCUI2C_INITERR
     \sa 
*/
/*=============================================================================================================*/
static void cdc_start_cmdproc( int state, int signal )
{
    send_message( code_in, _RESULT_OK, CMD_FSM, CDCRECV_FSM, cmd_data_in );
    cmd_data_index = 0;
    cdc_curnt_signal = CDC_READY;	
}


/*=============================================================================================================*/
/*!  \brief

\return i2c_errcode_t
\retval MCUI2C_OK, MCUI2C_INITERR
\sa 
*/
/*=============================================================================================================*/
static void cdc_reset( int state, int signal )
{	
    cmd_data_index = 0;
    cdc_curnt_signal = CDC_READY;
}



/*=============================================================================================================*/
/*!  \brief

\return 
\retval 
\sa 
*/
/*=============================================================================================================*/
enum CDCSEND_STATE*	get_curst_sendcdc (void)
    {
    return &cdc_curnt_sendstate ;
    };






/*=============================================================================================================*/
/*!  \brief

\return 
\retval 
\sa 
*/
/*=============================================================================================================*/
int	 get_cursig_sendcdc( void )
    {
    adress_code_t       answer_addr;
        
    switch ( cdc_curnt_sendstate )
        {
        case   CDCSEND_STATE_READY:
                if ( recv_message( CDCSEND_FSM, &answ_code, &answmsg_code, &answer_addr, &answ_data ) == OK )
                    {
                    switch ( answmsg_code )
                        {
                            case CDMCODE_GET_STATUS:                                
                            case CMDCODE_GET_PRES_STS:
                            case CMDCODE_GET_YIG:
                            case CMDCODE_GET_ATT0:
                            case CMDCODE_GET_IFSTS:
                            case CMDCODE_GETUNO:
                                return cdc_curnt_sendsignal = CDC_DATA; 
                                
                            case CMDCODE_FREQ:
                            case CMDCODE_SETCODE:
                            case CMDCODE_SETDAC:
                            case CMDCODE_DDS:
                            case CMDCODE_ATT:
                            case CMDCODE_PRES:
                            case CMDCODE_YIG:
                            case CMDCODE_SET_ATT0:
                            case CMDCODE_SET_IF:                                
                            case CMDCODE_UNO:
                            default:
                                return cdc_curnt_sendsignal = CDC_CODE;                                
                        }
                    }
                break;  
                
        case   CDCSEND_STATE_DATASEND:  
                return cdc_curnt_sendsignal = CDC_CODE;                                    
                
        case   CDCSEND_STATE_CODEOFOP:  
                return cdc_curnt_sendsignal = CDC_READY;                                    
        }
                
    return cdc_curnt_sendsignal;                
    }



/*=============================================================================================================*/
/*!  \brief

\return 
\retval 
\sa 
*/
/*=============================================================================================================*/
static void cdc_send_data( int state, int signal )
    {
    uint32_t    len = 0;
    char        buf[256];
    
    switch ( answmsg_code )
        {
            case CDMCODE_GET_STATUS:
                {
                
                custs_flags_t temp_sts_flags;
                uint16_t       code;
                
                /* ñòàòóñ ÁÓ */
                temp_sts_flags.byteflags = answ_data[0];
                
                if ( temp_sts_flags.bitflags.alarm )
                    {
                    len += sprintf( &buf[len], "\r\novrcrnt" );
                    }
                else
                    {
                    len += sprintf( &buf[len], "\r\nok" );
                    }
                
                switch ( temp_sts_flags.bitflags.type )
                    {
                        case CU_TYPE_NOISE_GEN :    len += sprintf( &buf[len], " sweep" ); break;
                        case CU_TYPE_YIG:           len += sprintf( &buf[len], " yig" ); break;
                        case CU_TYPE_PRESELECTOR:   len += sprintf( &buf[len], " pres" ); break;
                        case CU_TYPE_HETERODIN:     len += sprintf( &buf[len], " het" );  break;
                        case CU_TYPE_IF:            len += sprintf( &buf[len], " infreq" ); break;
                    }
   
                /* ÖÀÏ0 */
                memcpy( &code, &((uint8_t*)answ_data)[2], 2 );
	            len += sprintf(&buf[len], " %03X", code);
    
                /* ÖÀÏ1 */
                memcpy( &code, &((uint8_t*)answ_data)[4], 2 );
                len += sprintf( &buf[len], " %03X", code );
    
                /* ÖÀÏ2 */
                memcpy( &code, &((uint8_t*)answ_data)[6], 2 );
                len += sprintf( &buf[len], " %03X", code );

                    /* ÖÀÏ3 */
                memcpy( &code, &((uint8_t*)answ_data)[8], 2 );
	                len += sprintf(&buf[len], " %03X", code);

                    /* ÖÀÏ4 */
                memcpy( &code, &((uint8_t*)answ_data)[10], 2 );
	                len += sprintf(&buf[len], " %03X", code);
    
                /* ÖÀÏ5 */
                memcpy( &code, &((uint8_t*)answ_data)[12], 2 );
	                len += sprintf(&buf[len], " %03X", code);
    
                /* ÖÀÏ6 */
                memcpy( &code, &((uint8_t*)answ_data)[14], 2 );
	                len += sprintf(&buf[len], " %03X", code);
    
                /* ÖÀÏ7 */
                memcpy( &code, &((uint8_t*)answ_data)[16], 2 );
	                len += sprintf(&buf[len], " %03X", code);

                /* ÀÖÏ0 */
                memcpy( &code, &((uint8_t*)answ_data)[18], 2 );
	                len += sprintf(&buf[len], " %03X", code);
    
                /* ÀÖÏ1 */
                memcpy( &code, &((uint8_t*)answ_data)[20], 2 );
	                len += sprintf(&buf[len], " %03X", code);

                
                }
                break;
                
            case CMDCODE_GET_PRES_STS:
                {
                att_flags_t  att_flags;
                
                len = sprintf(&buf[len], "\r\n%d", answ_data[0]);
                
                memcpy( &att_flags.bitflags, &answ_data[1], 1 ); 
                if ( att_flags.bitflags.on )
                    {
                    len += sprintf( &buf[len], " on" );
                    }
                else
                    {
                    len += sprintf( &buf[len], " off" ); 
                    }
                
                if ( att_flags.bitflags.lna)
                    {
                    len += sprintf( &buf[len], " lnaon" );
                    }
                else
                    {
                    len += sprintf( &buf[len], " lnaoff" );                    
                    }
                
                if ( att_flags.bitflags.preslna) {
                    len += sprintf( &buf[len], " plnaon" );
                    }
                else {
                    len += sprintf( &buf[len], " plnaoff" );                    
                    }
                
                if ( att_flags.bitflags.test ) {
                    len += sprintf( &buf[len], " tston" );
                    }
                else {
                    len += sprintf( &buf[len], " tstoff" );                    
                    }
                
                }
                break;
                
            case CMDCODE_GET_YIG:
		        {
		        if ( answ_data[0] )
    		        {
    		        len = sprintf( &buf[len], "\r\non");
    		        }
		        else
    		        {
    		        len = sprintf( &buf[len], "\r\noff");    		        
    		        }
		        }
                break;
                
            case CMDCODE_GET_ATT0:
                {
                    uint16_t  gain;
                
                    gain = (uint16_t)answ_data[0] * 25;
                    len = sprintf( &buf[len], "\r\n%04d", gain );
                    
                    gain = (uint16_t)answ_data[1] * 25;
                    len += sprintf( &buf[len], " %04d", gain );
                    
                    gain = (uint16_t)answ_data[2] * 25;
                    len += sprintf( &buf[len], " %04d", gain );
                }
                break;
                
            case CMDCODE_GET_IFSTS:
                {
                    if_flags_t  if_flags;
                
                    memcpy( &if_flags.byteflags, &answ_data[0], 1 ); 

                    switch ( if_flags.bitflags.cispr_state )
                    {
                        case CISPR_OFF:         len = sprintf( &buf[len], "\r\noff");    break;
                        case CISPR_BPF9k:       len = sprintf( &buf[len], "\r\nBPF9k" ); break;
                        case CISPR_BPF120k:     len = sprintf( &buf[len], "\r\nBPF120k" ); break;
                        case CISPR_BPF1000k:    len = sprintf( &buf[len], "\r\nBPF1000k" ); break;
                    }
                
                    if ( if_flags.bitflags.filtr1012_on ) {
                        len += sprintf( &buf[len], " sawon" );
                        }
                    else {
                        len += sprintf( &buf[len], " sawoff" );                    
                        }
                    
                    if ( if_flags.bitflags.logamp_on ) {
                        len += sprintf( &buf[len], " laon" );
                        }
                    else {
                        len += sprintf( &buf[len], " laoff" );                    
                        }
                
                    if ( if_flags.bitflags.adc_if ) {
                        len += sprintf( &buf[len], " adc3" );
                        }
                    else {
                        len += sprintf( &buf[len], " adc2" );                    
                        }
                    
                    if ( if_flags.bitflags.rf_adc ) {
                        len += sprintf( &buf[len], " rfon" );
                        }
                    else {
                        len += sprintf( &buf[len], " rfoff" );                    
                        }
                }
                break;
                
            case CMDCODE_GETUNO:
                {
                    het_flags_t     het_flags;
                    uint16_t        gain;
                    uint32_t        freq;
                
                    memcpy( &het_flags.byteflags, &answ_data[0], 1 ); 
                    switch ( het_flags.bitflags.mulstate ) {
                        case HETMUL_OFF:        len = sprintf( &buf[len], "\r\noff" );      break;
                        case HETMUL_X2LOW:      len = sprintf( &buf[len], "\r\nx2low" );    break;
                        case HETMUL_X2HIGH:     len = sprintf( &buf[len], "\r\nx2high" );   break;
                        case HETMUL_X4HIGH:     len = sprintf( &buf[len], "\r\nx4high" );   break;
                    }
                
                    gain = (uint16_t)answ_data[1] * 5;
                    len += sprintf( &buf[len], " %d", gain );
                    
                    memcpy( &freq, &answ_data[2], 4 );
                    len += sprintf( &buf[len], " %d", freq );

	                switch (het_flags.bitflags.mulstate) {
	                case HETMUL_OFF:        len += sprintf(&buf[len], " off");    break;
	                case HETMUL_X2LOW:      len += sprintf(&buf[len], " x2low");  break;
	                case HETMUL_X2HIGH:     len += sprintf(&buf[len], " x2high"); break;
	                case HETMUL_X4HIGH:     len += sprintf(&buf[len], " x4high"); break;
	                }
	                
	                gain = (uint16_t)answ_data[7] * 5;
	                len += sprintf(&buf[len], " %d", gain);
	                
	                memcpy(&freq, &answ_data[8], 4);
	                len += sprintf(&buf[len], " %d", freq);
	                
                }
                break;
                
            default: 
                break;
        }
        VCP_write( buf, len );
    
    }

/*=============================================================================================================*/
/*!  \brief

\return 
\retval 
\sa 
*/
/*=============================================================================================================*/
//uint32_t    answer_col_ok  = 0;
//uint32_t    answer_col_err = 0;
//
static void cdc_send_code( int state, int signal )
    {
    uint32_t    len;
    uint8_t     buf[32];
    
    if ( answ_code == _RESULT_OK )
        {
//        answer_col_ok++;
        VCP_write("\r\nOK\r\n", 6);
        }
    else
        {
 //       answer_col_err++;
        len = sprintf( buf, "\r\nERR %02X\r\n", answ_code );
        VCP_write( buf, len );
        }    
    }

    