#ifndef		DAC_H
#define		DAC_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	cpld_bus.h
 *   \brief  файл функций работающих с шиной к CPLD
 */

   
/*includes==========================================================================================================*/
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>

/*defines===========================================================================================================*/
#ifndef  OK
#define  OK     (0)
#endif

#ifndef   ERR
#define   ERR     (-1)
#endif

	
#define  DAC_ALL	(0x0Fu)			/*	выбор ЦАП при инициализации	*/
#define  DAC_01		(0x01u)
#define  DAC_23		(0x02u)
#define  DAC_45		(0x04u)
#define  DAC_67		(0x08u)
	
/*types=============================================================================================================*/

    
/*prototypes========================================================================================================*/
int dac_open
(
	int	dac_
);
 
#ifdef	__cplusplus
	}
#endif

#endif /* DAC_H */

