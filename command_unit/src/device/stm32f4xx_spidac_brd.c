/*!  \file 	stm32f4xx_spidac_brd.c
 *   \brief  Файл работы с bidirectional spi
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "stm32f4xx_spidac_brd.h"
#include "stm32f4xx_gpio_brd.h"
#include "ad9116.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/

/*types=============================================================================================================*/

/*prototypes========================================================================================================*/

/*variables=========================================================================================================*/
static	SPI_HandleTypeDef	hspi2;
static	int					dac_spi_flags;

	
/*code==============================================================================================================*/

/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int spi_dac_open(
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67				*/
	)    
{
	if ( !dac_spi_flags )
	{
		
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
	}
	
		
	dac_spi_flags |= dac_index;
	
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int spi_dac_close(
	int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67				*/
	)    
{
	dac_spi_flags &= ~dac_index;

	if (!dac_spi_flags)
	{	
		if (HAL_SPI_DeInit(&hspi2) != HAL_OK)
		{
			return ERR;
		}
		
	}
		
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa IOCNTL_SPIDAC_RESET
*/
/*=============================================================================================================*/
int spi_dac_iocntl(
	int			dac_index,		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67				*/
	uint8_t		ioflag			/*!< [in]  флаг обозначающий необходимое дейтсвие c интерфейсом SPI->DAC	*/
)    
{
	uint32_t tickstart = 0;
	
	switch (ioflag) {
		
	case IOCNTL_SPIDAC_RESET:
		{
			/* (RESET / PINMD) Logic 0 */		
			gpio_dacspi_reset_down();		
			/* Get tick */ 
			tickstart = HAL_GetTick();

			  /* Disable the SPI and reset the CRC: the CRC value should be cleared
			     on both master and slave sides in order to resynchronize the master
			     and slave for their respective CRC calculation */

			    /* Disable TXE, RXNE and ERR interrupts for the interrupt process */
			__HAL_SPI_DISABLE_IT(&hspi2, (SPI_IT_TXE | SPI_IT_RXNE | SPI_IT_ERR));

				           /* Disable SPI peripheral */
			__HAL_SPI_DISABLE(&hspi2);

							/* Reset CRC Calculation */
			if (hspi2.Init.CRCCalculation == SPI_CRCCALCULATION_ENABLE)
			{
				SPI_RESET_CRC(&hspi2);
			}
			
		/*	A Logic 1 on Pin 35(RESET / PINMD), followed by a Logic 0,
			resets the SPI port timing to the initial state of the instruction
			cycle.
		*/
			while ( (HAL_GetTick() - tickstart) < 10) {
				continue;
			}
			
			gpio_dacspi_reset_up();
			

			/* Enable SPI peripheral */
			__HAL_SPI_ENABLE(&hspi2);
				
			hspi2.State = HAL_SPI_STATE_READY;
			
		} break;
	
		
	case IOCNTL_SPIDAC_RESTART:
		{
			if (HAL_SPI_DeInit(&hspi2) != HAL_OK)
			{
				return ERR;
			}
			
			/* (RESET / PINMD) Logic 0 */		
			gpio_dacspi_reset_down();		
			/* Get tick */ 
			tickstart = HAL_GetTick();
		
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

			/*	A Logic 1 on Pin 35(RESET / PINMD), followed by a Logic 0,
				resets the SPI port timing to the initial state of the instruction
				cycle.
			*/
			while ((HAL_GetTick() - tickstart) < 10) {
				continue;
			}
			
			gpio_dacspi_reset_up();
		
			if (HAL_SPI_Init(&hspi2) != HAL_OK)
			{
				return ERR;
			}								
		} break;
		
		case IOCNTL_SPIDAC_DISABLE: 
		{
			if ((hspi2.Instance->CR1 & SPI_CR1_SPE) == SPI_CR1_SPE) {
				__HAL_SPI_DISABLE(&hspi2);
			}			
		} break;
		
		case IOCNTL_SPIDAC_ENABLE: 
		{
			/* Check if the SPI is already enabled */ 
			if ((hspi2.Instance->CR1 & SPI_CR1_SPE) != SPI_CR1_SPE) {
			/* Enable SPI peripheral */
				__HAL_SPI_ENABLE(&hspi2);      
			}
		} break;
		
	}  /* switch (ioflag) */

	return OK;
}



/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
static int spidac_wait_flag_or_timeout
	(	
		uint32_t		flag,
		FlagStatus		status,
		uint32_t		timeout
	)
{
	uint32_t tickstart = 0;

  /* Get tick */ 
	tickstart = HAL_GetTick();

	timeout = (timeout == HAL_MAX_DELAY) ? HAL_MAX_DELAY : timeout;
	
	while ( (__HAL_SPI_GET_FLAG(&hspi2, flag) == RESET) == status )
	{
//		if ((timeout != 0) && ((HAL_GetTick() - tickstart) > timeout))	{
//				return -1;
//		}
		continue;
	}
	
	return 0;	
}
	




/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int spi_dac_write(
	int			dac_index,		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
	uint8_t		addr,			/*!< [in]  начальный адрес регистра микросхемы на чиная с которого происходит запись	*/
	uint8_t		*data,			/*!< [in]  данные передающиеся в ЦАП													*/
	uint8_t		num,			/*!< [in]  количество байт данных от 1 до 4												*/
	uint32_t	timeout			/*!< [in]  таймаут на передачу одного байта												*/
)    
{	
	uint8_t		instr_byte = 0;
	
	/* проверяем входные переменные на валидность */
	if ( (addr > AD9116_CLKMODE) && (addr != AD9116_VERSION) )
	{
		return ERR_SPIDAC_WRNGADRR;
	}

	if (data == NULL)
	{
		return ERR_SPIDAC_NULL;
	}
	
	if ((num == 0) || (num > 4))
	{
		return ERR_SPIDAC_DATAOUT;		
	}
	
	
/* передаем instruction byte */
	if (hspi2.State != HAL_SPI_STATE_READY)
	{
		return ERR_SPIDAC_BUSY;
	}

/* опускаем вниз соответсвующий CS */
	gpio_dacspi_cs_down(dac_index);
	
	/* формируем instruction byte  
				bit 7   = 0 - write
				bit 5-6 = num;
				bit 0-4 - addr
	*/			
	instr_byte = (((num - 1) << 5) & 0x60) | (addr & 0x1F ) ;
	instr_byte &= (0x7F);  /* write */
	
    /* Process Locked */
	__HAL_LOCK(&hspi2);

	/* Configure communication */
	hspi2.State      = HAL_SPI_STATE_BUSY_TX;
	hspi2.ErrorCode  = HAL_SPI_ERROR_NONE;
	hspi2.pTxBuffPtr = &instr_byte;
	hspi2.TxXferSize = 1;
	hspi2.TxXferCount = 1;	
	SPI_1LINE_TX(&hspi2);					/* Output enable in bidirectional mode */
	
	hspi2.Instance->DR = (*hspi2.pTxBuffPtr++);
	hspi2.TxXferCount--;

//	if (SPI_WaitOnFlagUntilTimeout(&hspi2, SPI_FLAG_TXE, RESET, timeout) != HAL_OK) { 
//		return HAL_TIMEOUT;
//	}
//	
	/* передаем байты данных */
	hspi2.pTxBuffPtr = data;
	hspi2.TxXferSize = 4;
	hspi2.TxXferCount = num;
		
	while (hspi2.TxXferCount > 0)
	{
		/* Wait until TXE flag is set to send data */
		if (spidac_wait_flag_or_timeout(SPI_FLAG_TXE, RESET, timeout) != 0) { 
			hspi2.ErrorCode |= HAL_SPI_ERROR_FLAG;
			/* поднимаем соответсвующий CS */
			gpio_dacspi_cs_up(dac_index);
			/* Process Unlocked */
			__HAL_UNLOCK(&hspi2);			
			return ERR_SPIDAC_TIMEOUT;
		}
		hspi2.Instance->DR = (*hspi2.pTxBuffPtr++);
		hspi2.TxXferCount--;
	}
	
    /* Wait until Busy flag is reset before disabling SPI */
	if (spidac_wait_flag_or_timeout(SPI_FLAG_BSY, SET, timeout) != 0)
	{ 
		hspi2.ErrorCode |= HAL_SPI_ERROR_FLAG;
		/* поднимаем соответсвующий CS */
		gpio_dacspi_cs_up(dac_index);
		/* Process Unlocked */
		__HAL_UNLOCK(&hspi2);					
		return ERR_SPIDAC_TIMEOUT;
	}
 
	hspi2.State = HAL_SPI_STATE_READY; 

	/* поднимаем соответсвующий CS */
	gpio_dacspi_cs_up(dac_index);
	
	    /* Process Unlocked */
	__HAL_UNLOCK(&hspi2);

	return OK;
}



/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int spi_dac_read(
	int			dac_index,		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC_01..DAC67							*/
	uint8_t		addr,			/*!< [in]  начальный адрес регистра микросхемы на чиная с которого происходит запись	*/
	uint8_t		*data,			/*!< [out] данные передающиеся из ЦАП													*/
	uint8_t		num,			/*!< [in]  количество байт данных от 1 до 4												*/
	uint32_t	timeout			/*!< [in]  таймаут на передачу одного байта												*/
	)    
{
	uint8_t		instr_byte = 0;

	if (data == NULL)
	{
		return ERR_SPIDAC_NULL;
	}
	
/* передаем instruction byte */
	if (hspi2.State != HAL_SPI_STATE_READY)
	{
		return ERR_SPIDAC_BUSY;
	}

/* опускаем вниз соответсвующий CS */
	gpio_dacspi_cs_down(dac_index);
	
	/* формируем instruction byte  
				bit 7   = 1 - read
				bit 5-6 = num;
				bit 0-4 - addr
	*/			
	instr_byte = (((num - 1) << 5) & 0x60) | (addr & 0x1F);
	instr_byte |= (0x80);  /* read */
	
    /* Process Locked */
	__HAL_LOCK(&hspi2);

	/* Configure communication */
	hspi2.State      = HAL_SPI_STATE_BUSY_TX;
	hspi2.ErrorCode  = HAL_SPI_ERROR_NONE;
	hspi2.pTxBuffPtr = &instr_byte;
	hspi2.TxXferSize = 1;
	hspi2.TxXferCount = 1;	
	SPI_1LINE_TX(&hspi2);					/* Output enable in bidirectional mode */
	
	hspi2.Instance->DR = (*hspi2.pTxBuffPtr++);
	hspi2.TxXferCount--;

	if (spidac_wait_flag_or_timeout(SPI_FLAG_BSY, SET, timeout) != 0)
	{ 
		hspi2.ErrorCode |= HAL_SPI_ERROR_FLAG;
		/* поднимаем соответсвующий CS */
		gpio_dacspi_cs_up(dac_index);
		/* Process Unlocked */
		__HAL_UNLOCK(&hspi2);					
		return ERR_SPIDAC_TIMEOUT;
	}
		
	hspi2.pRxBuffPtr  = data;
	hspi2.RxXferSize  = 4;
	hspi2.RxXferCount = num;

	    /*Init field not used in handle to zero */
	hspi2.RxISR = 0;
	hspi2.TxISR = 0;
	hspi2.TxXferSize   = 0;
	hspi2.TxXferCount  = 0;
	
	/* reconfig spi to falling edge CPOL = 1 */
	hspi2.Instance->CR1 |= SPI_CR1_CPOL;
		
	SPI_1LINE_RX(&hspi2);					/* input enable in bidirectional mode */
	
    /* Receive data in 8 Bit mode */
	while (hspi2.RxXferCount > 0)
	{
	  /* Wait until RXNE flag is set */
		if (spidac_wait_flag_or_timeout(SPI_FLAG_RXNE, RESET, timeout) != 0)
		{ 
			hspi2.ErrorCode |= HAL_SPI_ERROR_FLAG;
			/* поднимаем соответсвующий CS */
			gpio_dacspi_cs_up(dac_index);
			/* Process Unlocked */
			__HAL_UNLOCK(&hspi2);					
			return ERR_SPIDAC_TIMEOUT;
		}

		(*hspi2.pRxBuffPtr++) = hspi2.Instance->DR;
		hspi2.RxXferCount--;
	}
	
///WTF?  for CS?
//	if((hspi->Init.Mode == SPI_MODE_MASTER)&&((hspi->Init.Direction == SPI_DIRECTION_1LINE)||(hspi->Init.Direction == SPI_DIRECTION_2LINES_RXONLY)))
//	{
//	  /* Disable SPI peripheral */
//		__HAL_SPI_DISABLE(hspi);
//	}
//
	/* поднимаем соответсвующий CS */
	gpio_dacspi_cs_up(dac_index);
	hspi2.State = HAL_SPI_STATE_READY;
	

	/* reconfig spi to rising edge  CPOL = 0 */
	hspi2.Instance->CR1 &= ~SPI_CR1_CPOL;
	
	
	/* Process Unlocked */
	__HAL_UNLOCK(&hspi2);
	
	return OK;
}
