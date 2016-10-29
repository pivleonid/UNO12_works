#ifndef CMD_PROC_H_
#define CMD_PROC_H_

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	cmd_proc.h
 *   \brief  
 */

/*includes==========================================================================================================*/
#include <stdint.h>
#include <string.h>
#include "src/mdlwr/fsm/fsm.h"



/*defines===========================================================================================================*/
    
	

/*types=============================================================================================================*/
typedef enum CMD_CODE
    {
/* main */    
        CMDCODE_FREQ            = (0xFFu),
        CDMCODE_GET_STATUS      = (0x70u),
/* low level */                
        CMDCODE_SETCODE         = (0xF1u),
        CMDCODE_SETDAC          = (0xF2u),
        CMDCODE_DDS             = (0xF3u),
/* preselector */
        CMDCODE_ATT             = (0x41u),
        CMDCODE_PRES            = (0x42u),
        CMDCODE_GET_PRES_STS    = (0x71u),
/*yig*/    
        CMDCODE_YIG             = (0x47u),
        CMDCODE_GET_YIG         = (0x77u),
/* interfreq 1, 2 and 3 */    
        CMDCODE_SET_ATT0        = (0x48u),
        CMDCODE_SET_IF          = (0x49u),
        CMDCODE_GET_ATT0        = (0x78u),
        CMDCODE_GET_IFSTS       = (0x79u),
/* heterodin */    
        CMDCODE_UNO             = (0x4Du),
        CMDCODE_GETUNO          = (0x7Du),
    } cmd_code_t;

    
typedef enum CMD_ERRCODE
    {
    CMD_ERRCODE_OK =    0,
    CMD_ERRCODE_FAIL = -1    
    }cmd_errcode_t;

/*prototypes========================================================================================================*/
PT_THREAD( pt_cmd_proc( struct pt *pt ) );
    
uint8_t get_code_from_string    ( char* );
int get_argnumfromcode          ( uint8_t code );

uint8_t	trans_data_from_string
(
 uint8_t    cmdcode, 
 char*	    string,
 char**     end, 
 void*      output_buf,
 size_t*     outlen  );
 
    	
#ifdef	__cplusplus
	}
#endif

#endif /* CMD_PROC_H_ */
