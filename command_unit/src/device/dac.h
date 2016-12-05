#ifndef		DAC_H
#define		DAC_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	cpld_bus.h
 *   \brief  ���� ������� ���������� � ����� � CPLD
 */

   
/*includes==========================================================================================================*/
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include "hrdwr_func.h"

/*defines===========================================================================================================*/
#ifndef  OK
#define  OK     (0)
#endif

#ifndef   ERR
#define   ERR     (-1)
#endif

	
#define  DAC_ALL	(0x0Fu)			/*	����� ��� ��� �������������	*/
#define  DAC0		(0x01u)
#define  DAC1		(0x02u)
#define  DAC2		(0x04u)
#define  DAC3		(0x08u)
	
/*types=============================================================================================================*/

    
/*prototypes========================================================================================================*/
int		dac_open
(
	cu_type_sts_t	type
);
	
int     dac_write
(
	uint8_t		index,	/*!< [in] ������ ���� ������������ �� 0-7		*/
	uint16_t	code	/*!< [in] ��� ��������� � ������������� ������	*/
);
	
int     dac_read
(
	uint8_t		index,	/*!< [in] ������ ���� ������������ �� 0-7		*/
	uint16_t	*code	/*!< [in] ��� ��������� � ������������� ������	*/
);
	
 
#ifdef	__cplusplus
	}
#endif

#endif /* DAC_H */

