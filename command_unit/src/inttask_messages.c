/* Sep 16, 2015 */

/*!  \file 	fsm_protocol_cdc.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "inttask_messages.h"
#include "cmd_proc.h"

/*defines===========================================================================================================*/
#define CDCMSG_BUFF_LEN  (16)
#define CMDMSG_BUFF_LEN  (16)


/*types=============================================================================================================*/
typedef struct _MSG {
    cmd_code_t      code;
    err_code_t      result_code;
    adress_code_t   source;
    void*           msg_data;
    }msg_t;


/*prototypes========================================================================================================*/


/*variables=========================================================================================================*/
CIRCBUFFER( cmdmsg_buff, CMDMSG_BUFF_LEN ); // размер буфера знать нигде не нужно!
CIRCBUFFER( cdc_buff, CDCMSG_BUFF_LEN ); // размер буфера знать нигде не нужно!

/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
static int _send_message( msg_t *input_msg, circ_buffer_t *circbuf_desc )
    {
    size_t  act_size;
    static msg_t   msg; 
    
    if ( circbuffer_get_space_size( circbuf_desc ) < sizeof *input_msg ) {
        return ERROR;
        }
    return circbuffer_push_block( circbuf_desc, &act_size, input_msg, sizeof msg );
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
static msg_t* _get_message( circ_buffer_t *circbuf_desc )
    {    
    static msg_t   msg; 
    size_t          act_size;
    
    if ( circbuffer_get_storage_data_size( circbuf_desc ) == 0 ) { 
        return NULL;
        }
    circbuffer_pop_block( circbuf_desc, (CircBuffer_datatype*)&msg, sizeof msg, &act_size );
    return &msg;
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
int send_message( cmd_code_t msg_code, err_code_t rescode, adress_code_t dest, adress_code_t source, void *data )
    {
    msg_t           msg;
    circ_buffer_t   *circbuf;
    
    msg.code        = msg_code;
    msg.source      = source;
    msg.msg_data    = data;
    msg.result_code = rescode;
    
    switch ( dest ) {
            case CDCSEND_FSM:   circbuf = &cdc_buff; break;
            case CMD_FSM:       circbuf = &cmdmsg_buff; break;
            default:            return ERROR;
        }
    
    return _send_message( &msg, circbuf );
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
int recv_message( adress_code_t dest, err_code_t *rescode, cmd_code_t *msg_code, adress_code_t *source, void **data )
    {
    circ_buffer_t   *circbuf;
    msg_t           *msg;
    
    switch ( dest ) {
            case CDCSEND_FSM:   circbuf = &cdc_buff; break;
            case CMD_FSM:       circbuf = &cmdmsg_buff; break;
            default:            return ERROR;
        }
    
    msg = _get_message( circbuf );
    
    if ( msg == NULL ) {
        return ERROR;
        }
    
    *msg_code       = msg->code;
    *source         = msg->source;
    *data           = msg->msg_data;
    *rescode        = msg->result_code;
    
    return OK;
    }


