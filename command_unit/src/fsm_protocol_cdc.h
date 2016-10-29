
/* Sep 16, 2015 */

#ifndef FSM_PROTOCOL_CDC_H_
#define FSM_PROTOCOL_CDC_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	fsm_protocol_cdc.h
 *   \brief  
 */

/*includes==========================================================================================================*/
#include <stdint.h>
#include "src/mdlwr/fsm/fsm.h"



/*defines===========================================================================================================*/


/*types=============================================================================================================*/
enum CDCRECV_STATE 
	{
	CDCRECV_STATE_READY     = 0,
	CDCRECV_STATE_CODEPARSE,
	CDCRECV_STATE_DATASAVE
	};

enum CDCSEND_STATE 
	{
	CDCSEND_STATE_READY    = 0,
	CDCSEND_STATE_DATASEND,
	CDCSEND_STATE_CODEOFOP
	};
    
	typedef enum CDC_ERRCODE 
	{
	CDCERR_OK       = 0,
	CDCERR_TIMEOUT,
	CDCERR_INITERR,	
	CDCERR_
	} cdc_errcode_t;

	

/*prototypes========================================================================================================*/
	extern const  fsm_table_t   cdc_recv_table[3][6];
    extern const  fsm_table_t   cdc_send_table[3][3];

	enum CDCRECV_STATE*	get_curst_cdc (void);
	int					get_cursig_cdc(void);
    enum CDCSEND_STATE*	get_curst_sendcdc (void);
    int	                get_cursig_sendcdc(void);


	
#ifdef	__cplusplus
}
#endif

#endif /* FSM_PROTOCOL_CDC_H_ */
