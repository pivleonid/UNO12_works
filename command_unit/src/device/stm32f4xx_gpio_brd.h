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

	
#define DAC_CLK_Pin GPIO_PIN_0
#define DAC_CLK_GPIO_Port GPIOB
#define DAC_RES_Pin GPIO_PIN_1
#define DAC_RES_GPIO_Port GPIOB
#define DAC_CLK_IO_0_Pin GPIO_PIN_12
#define DAC_CLK_IO_0_GPIO_Port GPIOE
#define DAC_CLK_IO_1_Pin GPIO_PIN_13
#define DAC_CLK_IO_1_GPIO_Port GPIOE
#define DAC_CLK_IO_2_Pin GPIO_PIN_14
#define DAC_CLK_IO_2_GPIO_Port GPIOE
#define DAC_CLK_IO_3_Pin GPIO_PIN_15
#define DAC_CLK_IO_3_GPIO_Port GPIOE

	
#define DAC_SCK_Pin GPIO_PIN_13
#define DAC_MISO_Pin GPIO_PIN_14
#define DAC_MOSI_Pin GPIO_PIN_15
#define DAC_SPI_GPIO_Port GPIOB
	
#define DAC_SPI_SS0_Pin GPIO_PIN_12
#define DAC_SPI_SS0_GPIO_Port GPIOB
#define DAC_SPI_SS3_Pin GPIO_PIN_3
#define DAC_SPI_SS3_GPIO_Port GPIOB
#define DAC_SPI_SS2_Pin GPIO_PIN_4
#define DAC_SPI_SS2_GPIO_Port GPIOB
#define DAC_SPI_SS1_Pin GPIO_PIN_5
#define DAC_SPI_SS1_GPIO_Port GPIOB

	
/*types=============================================================================================================*/
    
/*prototypes========================================================================================================*/

	int gpio_cpld_open( void );    
    int gpio_cpld_close( void );
    
	int gpio_dac_open(int);
	int gpio_dac_close(int);

    int  gpio_dacspi_open(int);
	int  gpio_dacspi_close(int);
	void gpio_dacspi_cs_down(int);
	void gpio_dacspi_cs_up(int);
	void gpio_dacspi_reset_up(void);
	void gpio_dacspi_reset_down(void);
	
	int  gpio_dac0_write(uint16_t	data_i, uint16_t data_q);
	int  gpio_dac1_write(uint16_t	data_i, uint16_t data_q);
	int  gpio_dac2_write(uint16_t	data_i, uint16_t data_q);
	int  gpio_dac3_write(uint16_t	data_i, uint16_t data_q);
	int  gpio_dac_write_all(uint16_t *data, uint16_t len);
    
	int gpio_dacpwm_open(void);
	int gpio_dacpwm_close(void);

	int gpio_dac_pinmode
		( 
		int			dac_index		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC0..DAC67							*/
		);

#ifdef	__cplusplus
	}
#endif

#endif /* STM32F4XX_GPIO_BRD_H */
