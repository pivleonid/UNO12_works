#ifndef STM32F4xx_SPIDAC_BRD_H
#define STM32F4xx_SPIDAC_BRD_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	stm32f4xx_spidac_brd.h
 *   \brief  файл функций двухпроводного spi с двунапаравленной передачей в режиме "мастер"
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

#define   ERR_SPIDAC_WRNGADRR	(-2)
#define   ERR_SPIDAC_DATAOUT	(-3)
#define   ERR_SPIDAC_NULL		(-4)
#define   ERR_SPIDAC_BUSY		(-5)
#define	  ERR_SPIDAC_TIMEOUT	(-6)

#define   IOCNTL_SPIDAC_RESET		(0x1u)
#define	  IOCNTL_SPIDAC_RESTART		(0x2u)
#define   IOCNTL_SPIDAC_DISABLE		(0x3u)
#define   IOCNTL_SPIDAC_ENABLE		(0x4u)
	
/*types=============================================================================================================*/
    
/*prototypes========================================================================================================*/

	int spi_dac_open(int);    
	int spi_dac_close(int);
	int spi_dac_iocntl(
		int			dac_index,		/*!< [in]  определяет индекс микросхемы ЦАП БУ								*/
		uint8_t		ioflag 			/*!< [in]  флаг обозначающий необходимое дейтсвие c интерфейсом SPI->DAC	*/    
	);
	int spi_dac_write(
		int			dac_index,		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC0..DAC67							*/
		uint8_t		addr,			/*!< [in]  начальный адрес регистра микросхемы на чиная с которого происходит запись	*/
		uint8_t		*data,			/*!< [in]  данные передающиеся в ЦАП													*/
		uint8_t		num,			/*!< [in]  количество байт данных от 1 до 4												*/
		uint32_t	timeout			/*!< [in]  таймаут на передачу одного байта 											*/
	);
	int spi_dac_read(
		int			dac_index,		/*!< [in]  определяет индекс микросхемы ЦАП БУ	DAC0..DAC67							*/
		uint8_t		addr,			/*!< [in]  начальный адрес регистра микросхемы на чиная с которого происходит запись	*/
		uint8_t		*data,			/*!< [out] данные передающиеся из ЦАП													*/
		uint8_t		num,			/*!< [in]  количество байт данных от 1 до 4												*/
		uint32_t	timeout			/*!< [in]  таймаут на передачу одного байта												*/
	);

#ifdef	__cplusplus
}
#endif

#endif /* STM32F4xx_SPIDAC_BRD_H */
