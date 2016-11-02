/*!  \file 	dac.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "ad9116.h"
#include "stm32f4xx_gpio_brd.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/
static void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


/*variables=========================================================================================================*/
static	uint8_t				clock_timer_disabled;		/* флаг указывающий работает или нет тактирующий таймер 1-выключен, 0-включен*/
static	TIM_HandleTypeDef	htim3;
static	SPI_HandleTypeDef	hspi2;

/*code==============================================================================================================*/

/*=============================================================================================================*/
/*!  \brief конфигурируем интерфейсы последовательный и параралельный для управлениея ЦАПом

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_open
    (
     int dac_index      /*!< [in] индекс цапа 0-3 */
     )
    {
    
        /* конфигурирование ног паралелльной шины как выходов */
	    gpio_dac_open(dac_index);
    
        /* включение интерфейса SPI2 */
	    hspi2.Instance = SPI2;
	    hspi2.Init.Mode = SPI_MODE_MASTER;
	    hspi2.Init.Direction = SPI_DIRECTION_1LINE;
	    hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	    hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	    hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	    hspi2.Init.NSS = SPI_NSS_SOFT;
	    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	    hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	    hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	    hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	    hspi2.Init.CRCPolynomial = 10;
	    if (HAL_SPI_Init(&hspi2) != HAL_OK)
	    {
		    return ERR;
	    }

	    gpio_dacspi_open(dac_index);
        
	   	    
        /* включение тактирующего таймера */
	    if (clock_timer_disabled)
	    {
		    clock_timer_disabled = 0;
		    /* tim3 ch3 */
		    TIM_ClockConfigTypeDef	sClockSourceConfig;
		    TIM_MasterConfigTypeDef sMasterConfig;
		    TIM_OC_InitTypeDef		sConfigOC;

		    htim3.Instance = TIM3;
		    htim3.Init.Prescaler = 0;
		    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		    htim3.Init.Period = 0;
		    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		    if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
		    {
			    return ERR;
		    }

		    sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
		    if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
		    {
			    return ERR;
		    }

		    if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
		    {
			    return ERR;
		    }

		    sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
		    sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
		    if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
		    {
			    return ERR;
		    }

		    sConfigOC.OCMode = TIM_OCMODE_PWM1;
		    sConfigOC.Pulse = 0;
		    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
		    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		    if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
		    {
			    return ERR;
		    }

		    gpio_dacpwm_open();		    
	}
    
	return OK;
    }



/*=============================================================================================================*/
/*!  \brief выключаем интерфейсы работаюшие с ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_close
    (
     int dac_index      /*!< [in] индекс цапа 0-3 */
     )
    {    
    /**/
	gpio_dac_close(dac_index);
	gpio_dacspi_close(dac_index);
		    
    if (!clock_timer_disabled)
    {
	    gpio_dacpwm_close();
	    
	    clock_timer_disabled = 1;
    }
	    
    return OK;    
    }



/*=============================================================================================================*/
/*!  \brief  Запись цифровых данных в ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_write
    (
     int                    dac_index,  /*!< [in] индекс цапа 0-3                           */
	 int					q_or_i,		/*!< [in] выбор выхода ЦАП							*/
     enum AD9116_INTERFACE  interface,  /*!< [in] интерфейс по которому передаются данные   */
     uint8_t                *data,      /*!< [in] указатель на передаваемые в ЦАП данные    */
     uint8_t                len         /*!< [in] длина передаваемых данных в байтах        */
     )
    {
    /**/
    
    return OK;    
    }
    



/*=============================================================================================================*/
/*!  \brief Чтение данных с ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_read
    (
     int                    dac_index,   /*!< [in] индекс цапа 0-3                           */
	 int					q_or_i,		 /*!< [in] выбор выхода ЦАП							*/
	 enum AD9116_INTERFACE  interface,   /*!< [in] интерфейс по которому передаются данные   */
     uint8_t                *data,       /*!< [in] указатель на передаваемые в ЦАП данные    */
     uint8_t                len          /*!< [in] длина передаваемых данных в байтах        */
     )
    {
    /**/
    
    return OK;    
    }
    

