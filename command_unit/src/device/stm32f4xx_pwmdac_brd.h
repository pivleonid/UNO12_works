#ifndef STM32F4xx_PWMDAC_BRD_H
#define STM32F4xx_PWMDAC_BRD_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	stm32f4xx_pwmdac_brd.h
 *   \brief  файл функций управления ШИМом тактирующим ЦАП
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
#define   ERR   (-1)
#endif	
	

	
	
/*types=============================================================================================================*/

	
/*prototypes========================================================================================================*/

int pwm_dac_open(void);
int pwm_dac_close(void);
int pwm_dac_enable(void);
int pwm_dac_disable(void);	

#ifdef	__cplusplus
}
#endif

#endif /* STM32F4xx_PWMDAC_BRD_H */

