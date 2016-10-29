#ifndef		AD9116_H
#define		AD9116_H

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
#include "ad9116r.h"
        
/*defines===========================================================================================================*/
#ifndef  OK
#define  OK     (0)
#endif

#ifndef   ERR
#define   ERR     (-1)
#endif

/*types=============================================================================================================*/
enum AD9116_INTERFACE {
    AD9116_SPI,
    AD9116_PARALLEL
    };
    
/*prototypes========================================================================================================*/

 
#ifdef	__cplusplus
	}
#endif

#endif /* AD9116_H */

