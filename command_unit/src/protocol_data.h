#ifndef PROTOCOL_DATA_H_
#define PROTOCOL_DATA_H_


#ifdef	__cplusplus
extern "C" {
#endif

    /*!  \file 	cmd_proc.h
    *   \brief
    */

/*includes==========================================================================================================*/
#include <stdint.h>



/*defines===========================================================================================================*/
#define	STORAGE_ATOMIC()	 ;  // TODO: надо бы перейти на хранение sr в стеке

#define	START_ATOMIC()		{;}
#define	STOP_ATOMIC()		{;}


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/
extern    uint8_t		cmd_data_in[4096];
extern    uint8_t       cmd_data_out[4096];
extern    uint8_t		cmd_data_index;


#ifdef	__cplusplus
    }
#endif

#endif /* PROTOCOL_DATA_H_ */
