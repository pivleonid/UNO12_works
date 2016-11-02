/*!  \file 	cpld_bus.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "stm32f4xx_gpio_brd.h"
#include "ad9116.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/

/*types=============================================================================================================*/

/*prototypes========================================================================================================*/
void gpio_port_write(GPIO_TypeDef*  GPIOx, uint16_t value);

/*variables=========================================================================================================*/
static SPI_HandleTypeDef	handle_spi;
static	int					dac_parbus_flags;
static	int					dac_working_flags;

/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief 

     \return int
*/
/*=============================================================================================================*/
void gpio_port_write(GPIO_TypeDef*  GPIOx, uint16_t value)
{
	GPIOx->ODR = value;	
}

	
	
/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_open
	( 
	int dac_index 
	)
	{
	    GPIO_InitTypeDef    GPIO_InitStructure;    
		
		/* включаем dac_reset и ставим высокий уровень */
		if (dac_parbus_flags == 0)		
		{
			if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN))
			{
				__GPIOB_CLK_ENABLE();		    
			}
			GPIO_InitStructure.Pin = DAC_RES_Pin;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(DAC_RES_GPIO_Port, &GPIO_InitStructure);        				
			HAL_GPIO_WritePin(DAC_RES_GPIO_Port, DAC_RES_Pin, GPIO_PIN_SET);			
		}
		
		dac_parbus_flags |= dac_index;
		
		/* включаем в любом случае так как на порту стробы данных */
		__GPIOE_CLK_ENABLE();
		
		if ( (dac_index & DAC_01) || (dac_index & DAC_67) )
		{
			/* включаем парраллельную шину */	
			GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
										GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
										GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);        
		
			/* устанавливаем состояние high */
			gpio_port_write(GPIOE, 0xFFF);
			
			/* включаем CS и CLKIO */
			if (dac_index & DAC_01)
			{
				GPIO_InitStructure.Pin = DAC_SPI_SS0_Pin;
				GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				HAL_GPIO_Init(DAC_SPI_SS0_GPIO_Port, &GPIO_InitStructure);        				
								
				GPIO_InitStructure.Pin = DAC_CLK_IO_0_Pin;
				GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				HAL_GPIO_Init(DAC_CLK_IO_0_GPIO_Port, &GPIO_InitStructure);        				
				
				/* устанавливаем состояние high */
				HAL_GPIO_WritePin(DAC_SPI_SS0_GPIO_Port, DAC_SPI_SS0_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DAC_CLK_IO_0_GPIO_Port, DAC_CLK_IO_0_Pin, GPIO_PIN_SET);
			}
			
			if (dac_index & DAC_67)
			{
				GPIO_InitStructure.Pin = DAC_SPI_SS3_Pin;
				GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				HAL_GPIO_Init(DAC_SPI_SS3_GPIO_Port, &GPIO_InitStructure);        
				
				GPIO_InitStructure.Pin = DAC_CLK_IO_3_Pin;
				GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
				GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
				GPIO_InitStructure.Pull = GPIO_NOPULL;
				HAL_GPIO_Init(DAC_CLK_IO_3_GPIO_Port, &GPIO_InitStructure);        
				/* устанавливаем состояние high */
				HAL_GPIO_WritePin(DAC_SPI_SS3_GPIO_Port, DAC_SPI_SS3_Pin, GPIO_PIN_SET);
				HAL_GPIO_WritePin(DAC_CLK_IO_3_GPIO_Port, DAC_CLK_IO_3_Pin, GPIO_PIN_SET);
			}
			
		}
		if (dac_index & DAC_23)
		{
			__GPIOC_CLK_ENABLE();
			GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
										GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
										GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
			/* устанавливаем состояние high */
			gpio_port_write(GPIOC, 0xFFF);


			/* включаем CS и CLKIO */
			GPIO_InitStructure.Pin =  DAC_SPI_SS1_Pin;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(DAC_SPI_SS1_GPIO_Port, &GPIO_InitStructure);        
						
			GPIO_InitStructure.Pin =  DAC_CLK_IO_1_Pin;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(DAC_CLK_IO_1_GPIO_Port, &GPIO_InitStructure);        
			
			/* устанавливаем состояние high */
			HAL_GPIO_WritePin(DAC_SPI_SS1_GPIO_Port, DAC_SPI_SS1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DAC_CLK_IO_1_GPIO_Port, DAC_CLK_IO_1_Pin, GPIO_PIN_SET);
		}
		if (dac_index & DAC_45)
		{
			__GPIOG_CLK_ENABLE();
			GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
										GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
										GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);            
			/* устанавливаем состояние high */
			gpio_port_write(GPIOG, 0xFFF);

			
			/* включаем CS и CLKIO */			
			GPIO_InitStructure.Pin = DAC_SPI_SS2_Pin;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(DAC_SPI_SS2_GPIO_Port, &GPIO_InitStructure);        
			
			GPIO_InitStructure.Pin = DAC_CLK_IO_2_Pin;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_InitStructure.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(DAC_CLK_IO_2_GPIO_Port, &GPIO_InitStructure);        
			
			/* устанавливаем состояние high */
			HAL_GPIO_WritePin(DAC_SPI_SS2_GPIO_Port, DAC_SPI_SS2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(DAC_CLK_IO_2_GPIO_Port, DAC_CLK_IO_2_Pin, GPIO_PIN_SET);
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
int gpio_dac_close
	( 
	int dac_index
	)
    {
        GPIO_InitTypeDef    GPIO_InitStructure;
	    
	    dac_parbus_flags &= ~dac_index;
		    
	    if ((dac_index & DAC_01) || (dac_index & DAC_67))
	    {
	    	/* включаем парраллельную шину */	
		    GPIO_InitStructure.Pin =  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
		                              GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
		                              GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);        
		    
				/* включаем CS и CLKIO */
		    if (dac_index & DAC_01)
		    {
			    GPIO_InitStructure.Pin = DAC_SPI_SS0_Pin;
			    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
			    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			    GPIO_InitStructure.Pull = GPIO_NOPULL;
			    HAL_GPIO_Init(DAC_SPI_SS0_GPIO_Port, &GPIO_InitStructure);        				
								
			    GPIO_InitStructure.Pin = DAC_CLK_IO_0_Pin;
			    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
			    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			    GPIO_InitStructure.Pull = GPIO_NOPULL;
			    HAL_GPIO_Init(DAC_CLK_IO_0_GPIO_Port, &GPIO_InitStructure);        				
		    }
			
		    if (dac_index & DAC_67)
		    {
			    GPIO_InitStructure.Pin = DAC_SPI_SS3_Pin;
			    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
			    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			    GPIO_InitStructure.Pull = GPIO_NOPULL;
			    HAL_GPIO_Init(DAC_SPI_SS3_GPIO_Port, &GPIO_InitStructure);        
				
			    GPIO_InitStructure.Pin = DAC_CLK_IO_3_Pin;
			    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
			    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			    GPIO_InitStructure.Pull = GPIO_NOPULL;
			    HAL_GPIO_Init(DAC_CLK_IO_3_GPIO_Port, &GPIO_InitStructure);        
		    }
			
	    }
	    if (dac_index & DAC_23)
	    {
		    GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
		                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
		                                GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
		    __GPIOC_CLK_DISABLE();		    

  			/* включаем CS и CLKIO */
		    GPIO_InitStructure.Pin =  DAC_SPI_SS1_Pin;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(DAC_SPI_SS1_GPIO_Port, &GPIO_InitStructure);        
						
		    GPIO_InitStructure.Pin =  DAC_CLK_IO_1_Pin;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(DAC_CLK_IO_1_GPIO_Port, &GPIO_InitStructure);        			
	    }
	    if (dac_index & DAC_45)
	    {
		    GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
		                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
		                                GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);            
		    __GPIOG_CLK_DISABLE();
		    
			
   			/* включаем CS и CLKIO */			
		    GPIO_InitStructure.Pin = DAC_SPI_SS2_Pin;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(DAC_SPI_SS2_GPIO_Port, &GPIO_InitStructure);        
			
		    GPIO_InitStructure.Pin = DAC_CLK_IO_2_Pin;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(DAC_CLK_IO_2_GPIO_Port, &GPIO_InitStructure);			
	    }

	    
	    
		/* если все тактирующие сигналы портов GPIO выключены выключаем reset  */
	    if (dac_parbus_flags == 0)		
		{
			GPIO_InitStructure.Pin = DAC_RES_Pin;
		    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    HAL_GPIO_Init(DAC_RES_GPIO_Port, &GPIO_InitStructure);        				
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
int gpio_dacspi_open
	( 
	int dac_index 
	)
{
	GPIO_InitTypeDef    GPIO_InitStructure;
    
	if (dac_working_flags == 0)
	{
		if (!(RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN))
		{
			__GPIOB_CLK_ENABLE();		    
		}		
		__SPI2_CLK_ENABLE();

		        /* линии данных */
		         /**SPI2 GPIO Configuration    
		            PB13     ------> SPI2_SCK
		            PB14     ------> SPI2_MISO
		            PB15     ------> SPI2_MOSI 
		   */
	//	GPIO_InitStructure.Pin =  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		GPIO_InitStructure.Pin =  DAC_SCK_Pin | DAC_MOSI_Pin;
		GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStructure.Pull = GPIO_NOPULL;
		GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
		HAL_GPIO_Init(DAC_SPI_GPIO_Port, &GPIO_InitStructure);			
	}
	dac_working_flags |= dac_index;
	
	    return OK;

    }

/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dacspi_close
	( 
	int dac_index 
	)
{
        GPIO_InitTypeDef    GPIO_InitStructure;

	    dac_working_flags &= ~dac_index;

	    if (dac_working_flags == 0)
	    {	    
		/* линии данных */
		 /**SPI2 GPIO Configuration    
		    PB13     ------> SPI2_SCK
		    PB14     ------> SPI2_MISO
		    PB15     ------> SPI2_MOSI 
		 */
//		    GPIO_InitStructure.Pin =  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
		    GPIO_InitStructure.Pin =  DAC_SCK_Pin | DAC_MOSI_Pin;
	        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
		    GPIO_InitStructure.Pull = GPIO_NOPULL;
		    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		    GPIO_InitStructure.Alternate = 0;
		    HAL_GPIO_Init(DAC_SPI_GPIO_Port, &GPIO_InitStructure);
    
		    __SPI2_CLK_DISABLE();
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
int gpio_dacpwm_open(void)
{

	GPIO_InitTypeDef	GPIO_InitStruct;
	
	    /**TIM3 GPIO Configuration    
	    PB0     ------> TIM3_CH3 
	    */
	GPIO_InitStruct.Pin = DAC_CLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(DAC_CLK_GPIO_Port, &GPIO_InitStruct);

	  /* USER CODE BEGIN TIM3_MspPostInit 1 */

    /* USER CODE END TIM3_MspPostInit 1 */
	return OK;
}



/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dacpwm_close(void)
{

	GPIO_InitTypeDef	GPIO_InitStruct;
	
	    /**TIM3 GPIO Configuration    
	    PB0     ------> TIM3_CH3 
	    */
	GPIO_InitStruct.Pin = DAC_CLK_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStruct.Alternate = 0;
	HAL_GPIO_Init(DAC_CLK_GPIO_Port, &GPIO_InitStruct);

	  /* USER CODE BEGIN TIM3_MspPostInit 1 */

    /* USER CODE END TIM3_MspPostInit 1 */

	return OK;

}
