#ifndef		STM32F4XX_GPIO_BRD_H
#define		STM32F4XX_GPIO_BRD_H

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

/*types=============================================================================================================*/
    
/*prototypes========================================================================================================*/

	int gpio_cpld_open( void );    
    int gpio_cpld_close( void );
    
    int gpio_dac_open( void );
    int gpio_dac_close( void );

    int gpio_dacspi_open( void );
    int gpio_dacspi_close( void );
    
    
#ifdef	__cplusplus
	}
#endif

#endif /* STM32F4XX_GPIO_BRD_H */
