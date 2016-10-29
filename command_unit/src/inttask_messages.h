#ifndef INTTASK_MESSAGES_H_
#define INTTASK_MESSAGES_H_


#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	cmd_proc.h
 *   \brief  
 */

/*includes==========================================================================================================*/
#include "mdlwr/common_lib/circ_buffer.h"
#include "cmd_proc.h"


/*defines===========================================================================================================*/
	

/*types=============================================================================================================*/
typedef enum _ADRESS_CODE
    {
    CDCRECV_FSM = 0,
    CDCSEND_FSM,
    CMD_FSM
    }adress_code_t;

typedef enum  _OPERATION_CODE
    {
    _RESULT_OK        = 0,
    _RESULT_BADVALUE  = 1,
    _RESULT_BADFORMAT = 2,
    _RESULT_NOOP      = 3
    } err_code_t;

   
/*prototypes========================================================================================================*/
int send_message( cmd_code_t msg_code, err_code_t rescode, adress_code_t dest, adress_code_t source, void *data );
int recv_message( adress_code_t dest, err_code_t *rescode, cmd_code_t *msg_code, adress_code_t *source, void **data );
    
    
	
#ifdef	__cplusplus
	}
#endif

#endif /* INTTASK_MESSAGES_H_ */
