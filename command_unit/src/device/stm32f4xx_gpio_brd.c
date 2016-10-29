/*!  \file 	cpld_bus.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "stm32f4xx_gpio_brd.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/

/*types=============================================================================================================*/

/*prototypes========================================================================================================*/

/*variables=========================================================================================================*/
static SPI_HandleTypeDef   handle_spi;


/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_open( void )
	{
	    GPIO_InitTypeDef    GPIO_InitStructure;
    
        /*  */  
    	__GPIOC_CLK_ENABLE();
    	GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	    GPIO_InitStructure.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init( GPIOC, &GPIO_InitStructure );
	
        /*  */  
    	__GPIOG_CLK_ENABLE();
	    GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
	    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	    GPIO_InitStructure.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init( GPIOG, &GPIO_InitStructure );            
    
        /*  */
	    __GPIOE_CLK_ENABLE();
        GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
	                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
	                                GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOE, &GPIO_InitStructure );        
		
	    /* CLKIO 0-3 */
    	GPIO_InitStructure.Pin =    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	    GPIO_InitStructure.Pull = GPIO_NOPULL;
	    HAL_GPIO_Init( GPIOE, &GPIO_InitStructure );        
	
    }

/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dac_close( void )
    {
        GPIO_InitTypeDef    GPIO_InitStructure;
    
        /*  */  
        GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOC, &GPIO_InitStructure );
        __GPIOC_CLK_DISABLE();
	
        /*  */  
        GPIO_InitStructure.Pin =    GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                                    GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOG, &GPIO_InitStructure );            
        __GPIOG_CLK_DISABLE();
        
    
        /* cs ы */
        GPIO_InitStructure.Pin =  GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 |
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOE, &GPIO_InitStructure );        
    
	    /* CLKIO 0-3 */
        GPIO_InitStructure.Pin =  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOE, &GPIO_InitStructure );        
        __GPIOE_CLK_DISABLE();
}

/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dacspi_open( void )
    {
        GPIO_InitTypeDef    GPIO_InitStructure;
    
        __SPI2_CLK_ENABLE();
        __HAL_RCC_SPI2_CLK_ENABLE();

        /* линии данных */
         /**SPI2 GPIO Configuration    
            PB13     ------> SPI2_SCK
            PB14     ------> SPI2_MISO
            PB15     ------> SPI2_MOSI 
         */
        GPIO_InitStructure.Pin =  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStructure );
    
        /* cs ы */
        GPIO_InitStructure.Pin =  GPIO_PIN_12 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStructure );
    }

/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int gpio_dacspi_close( void )
    {
        GPIO_InitTypeDef    GPIO_InitStructure;

        /* линии данных */
         /**SPI2 GPIO Configuration    
            PB13     ------> SPI2_SCK
            PB14     ------> SPI2_MISO
            PB15     ------> SPI2_MOSI 
         */
        GPIO_InitStructure.Pin =  GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStructure.Alternate = 0;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStructure );
    
        /* cs ы */
        GPIO_InitStructure.Pin =  GPIO_PIN_12 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
        GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOB, &GPIO_InitStructure );    
    
        __SPI2_CLK_DISABLE();
        __HAL_RCC_SPI2_CLK_DISABLE();    
    }

