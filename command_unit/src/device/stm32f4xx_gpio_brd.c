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
struct _CLKIO {
	uint8_t	clkio01:1;
	uint8_t	clkio23:1;
	uint8_t	clkio45:1;
	uint8_t	clkio67:1;
}; 
static union {
	uint8_t			byte;
	struct _CLKIO	bits;
}clkio = { 1, 1, 1, 1 };
	
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
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
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
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
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
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
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
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
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
void gpio_dacspi_cs_down
	(
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
	)
{
	if (dac_index & DAC_01)
	{
		HAL_GPIO_WritePin(DAC_SPI_SS0_GPIO_Port, DAC_SPI_SS0_Pin, GPIO_PIN_RESET);
	}
	else
	if (dac_index & DAC_23)
	{
		HAL_GPIO_WritePin(DAC_SPI_SS1_GPIO_Port, DAC_SPI_SS1_Pin, GPIO_PIN_RESET);		
	}
	else
	if (dac_index & DAC_45)	
	{
		HAL_GPIO_WritePin(DAC_SPI_SS2_GPIO_Port, DAC_SPI_SS2_Pin, GPIO_PIN_RESET);				
	}
	else
	if (dac_index & DAC_67)	
	{
		HAL_GPIO_WritePin(DAC_SPI_SS3_GPIO_Port, DAC_SPI_SS3_Pin, GPIO_PIN_RESET);				
	}
}


/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
void gpio_dacspi_cs_up
	(
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
	)
{
	if (dac_index & DAC_01)
	{
		HAL_GPIO_WritePin(DAC_SPI_SS0_GPIO_Port, DAC_SPI_SS0_Pin, GPIO_PIN_SET);
	}
	else
	if (dac_index & DAC_23)
	{
		HAL_GPIO_WritePin(DAC_SPI_SS1_GPIO_Port, DAC_SPI_SS1_Pin, GPIO_PIN_SET);		
	}
	else
	if (dac_index & DAC_45)	
	{
		HAL_GPIO_WritePin(DAC_SPI_SS2_GPIO_Port, DAC_SPI_SS2_Pin, GPIO_PIN_SET);				
	}
	else
	if (dac_index & DAC_67)	
	{
		HAL_GPIO_WritePin(DAC_SPI_SS3_GPIO_Port, DAC_SPI_SS3_Pin, GPIO_PIN_SET);				
	}
}


/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
void gpio_dacspi_reset_up (void)
{
	HAL_GPIO_WritePin(DAC_RES_GPIO_Port, DAC_RES_Pin, GPIO_PIN_SET);
}

/*=============================================================================================================*/
/*!  \brief 

     \sa 
*/
/*=============================================================================================================*/
void gpio_dacspi_reset_down(void)
{
	HAL_GPIO_WritePin(DAC_RES_GPIO_Port, DAC_RES_Pin, GPIO_PIN_RESET);
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


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_write_01
	(
	uint16_t	data_1,
	uint16_t	data_2
	)
{
	/* выставляем сначала data_1 */
	gpio_port_write(GPIOE, data_1 | (((uint16_t)clkio.byte) << 8));	
	/* CLKIO вниз  */		
	clkio.bits.clkio01 = 0;
	HAL_GPIO_WritePin(DAC_CLK_IO_0_GPIO_Port, DAC_CLK_IO_0_Pin, GPIO_PIN_RESET);

	/* выставляем data_2 */
	gpio_port_write(GPIOE, data_2 | (((uint16_t)clkio.byte) << 8));	
	
	/* CLKIO вверх  */		
	clkio.bits.clkio01 = 1;
	HAL_GPIO_WritePin(DAC_CLK_IO_0_GPIO_Port, DAC_CLK_IO_0_Pin, GPIO_PIN_SET);
		
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_write_23
	(
	uint16_t	data_1,
	uint16_t	data_2
	)
{
	/* выставляем сначала data_1 */
	gpio_port_write(GPIOC, data_1);	
	
	/* CLKIO вниз  */		
	clkio.bits.clkio23 = 0;
	HAL_GPIO_WritePin(DAC_CLK_IO_1_GPIO_Port, DAC_CLK_IO_1_Pin, GPIO_PIN_RESET);

	/* выставляем data_2 */
	gpio_port_write(GPIOC, data_2);	
	
	/* CLKIO вверх  */		
	clkio.bits.clkio23 = 1;
	HAL_GPIO_WritePin(DAC_CLK_IO_1_GPIO_Port, DAC_CLK_IO_1_Pin, GPIO_PIN_SET);
		
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_write_45
	(
	uint16_t	data_1,
	uint16_t	data_2
	)
{
	/* выставляем сначала data_1 */
	gpio_port_write(GPIOG, data_1 );	
	
	/* CLKIO вниз  */		
	clkio.bits.clkio45 = 0;
	HAL_GPIO_WritePin(DAC_CLK_IO_2_GPIO_Port, DAC_CLK_IO_2_Pin, GPIO_PIN_RESET);

	/* выставляем data_2 */
	gpio_port_write(GPIOG, data_2 );	
	
	/* CLKIO вверх  */		
	clkio.bits.clkio45 = 1;
	HAL_GPIO_WritePin(DAC_CLK_IO_2_GPIO_Port, DAC_CLK_IO_2_Pin, GPIO_PIN_SET);
			
	
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_write_67
	(
	uint16_t	data_1,
	uint16_t	data_2
	)
{
	/* выставляем сначала data_1 */
	gpio_port_write(GPIOE, data_1 | (((uint16_t)clkio.byte) << 8));	
	
	/* CLKIO вниз  */		
	clkio.bits.clkio67 = 0;
	HAL_GPIO_WritePin(DAC_CLK_IO_3_GPIO_Port, DAC_CLK_IO_3_Pin, GPIO_PIN_RESET);

	/* выставляем data_2 */
	gpio_port_write(GPIOE, data_2 | (((uint16_t)clkio.byte) << 8));	
	
	/* CLKIO вверх  */		
	clkio.bits.clkio67 = 1;
	HAL_GPIO_WritePin(DAC_CLK_IO_3_GPIO_Port, DAC_CLK_IO_3_Pin, GPIO_PIN_SET);
			
	
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_write_all
	(
	uint16_t	*data,
	uint16_t	len
	)
{
	/* 01 23 45 67 */
	/* выставляем сначала i */
	if ( len > 4 ) {  /* index 4-7 */
		clkio.bits.clkio45 = 0;
		gpio_port_write(GPIOG, data[4]);			
	}
	if (len > 2) {    /* index 2-8 */
		clkio.bits.clkio23 = 0;
		gpio_port_write(GPIOC, data[2]);					
	}
	if (len > 0) {	 /* index 0-8 */
		clkio.bits.clkio45 = 0;
		gpio_port_write(GPIOE, data[0] | ( ((uint16_t)clkio.byte) << 8) );
	} else	{
		return ERR;
	}
	
	if (len > 4) {  /* index 4-7 */
		clkio.bits.clkio45 = 1;
		gpio_port_write(GPIOG, data[5]);			
	}
	if (len > 2) {    /* index 2-8 */
		clkio.bits.clkio23 = 1;
		gpio_port_write(GPIOC, data[3]);					
	}	
	{	 /* index 0-8 */
		clkio.bits.clkio45 = 1;
		gpio_port_write(GPIOE, data[1] | (((uint16_t)clkio.byte) << 8));
	}
				
	if (len > 6) {  /* index 67 */
		clkio.bits.clkio67 = 0;		
		gpio_port_write(GPIOE, data[6] | (((uint16_t)clkio.byte) << 8));					
	} else {
		clkio.bits.clkio67 = 1;		
		gpio_port_write(GPIOE, data[7] | (((uint16_t)clkio.byte) << 8));							
	}
	
	return OK;
}





