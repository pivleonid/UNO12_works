#ifndef		CPLD_BUS_H
#define		CPLD_BUS_H

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

#define  CPLD_SELECT_CS0       (0u)
#define  CPLD_SELECT_CS1       (1u)
    
    
#define CPLD_D0         GPIO_PIN_0 
#define CPLD_D1         GPIO_PIN_1
#define CPLD_D2         GPIO_PIN_2
#define CPLD_D3         GPIO_PIN_3 
#define CPLD_D4         GPIO_PIN_4 
#define CPLD_D5         GPIO_PIN_5 
#define CPLD_D6         GPIO_PIN_6
#define CPLD_D7         GPIO_PIN_7

#define CPLD_WR         GPIO_PIN_12
#define CPLD_RD         GPIO_PIN_13
#define CPLD_CS0        GPIO_PIN_14
#define CPLD_CS1        GPIO_PIN_15


/*types=============================================================================================================*/

    
/*prototypes========================================================================================================*/

int cpld_open( void );    
int cpld_close( void );
int cpld_read( uint8_t, uint8_t*, int );
int cpld_write( uint8_t, uint8_t, int );
 
#ifdef	__cplusplus
    }
#endif

#endif /* CPLD_BUS_H */

    
    