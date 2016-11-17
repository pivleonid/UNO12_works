/*!  \file 	stm32f4xx_pwmdac_brd.c
 *   \brief  Ôאיכ נאבמעû ס pwm
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "stm32f4xx_pwmdac_brd.h"
#include "stm32f4xx_gpio_brd.h"
#include "ad9116.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/



/*variables=========================================================================================================*/
static	TIM_HandleTypeDef	htim3;

	
/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int pwm_dac_open(void)
{
	TIM_ClockConfigTypeDef	sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;
	TIM_OC_InitTypeDef		sConfigOC;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 0;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
		return ERR;
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		return ERR;
	}

	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		return ERR;
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
		return ERR;
	}

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
		return ERR;
	}
	
	return OK;	
}



/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int pwm_dac_close(void)
{
	
	
	return OK;		
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int pwm_dac_enable(void)
{
	
	
	return OK;		
}



/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int pwm_dac_disable(void)
{
	
	
	return OK;		
}




