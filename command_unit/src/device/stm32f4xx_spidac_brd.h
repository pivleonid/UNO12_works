#ifndef STM32F4xx_SPIDAC_BRD_H
#define STM32F4xx_SPIDAC_BRD_H

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	stm32f4xx_spidac_brd.h
 *   \brief  ���� ������� �������������� spi � ���������������� ��������� � ������ "������"
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
		int			dac_index,		/*!< [in]  ���������� ������ ���������� ��� ��								*/
		uint8_t		ioflag 			/*!< [in]  ���� ������������ ����������� �������� c ����������� SPI->DAC	*/    
	);
	int spi_dac_write(
		int			dac_index,		/*!< [in]  ���������� ������ ���������� ��� ��	DAC0..DAC67							*/
		uint8_t		addr,			/*!< [in]  ��������� ����� �������� ���������� �� ����� � �������� ���������� ������	*/
		uint8_t		*data,			/*!< [in]  ������ ������������ � ���													*/
		uint8_t		num,			/*!< [in]  ���������� ���� ������ �� 1 �� 4												*/
		uint32_t	timeout			/*!< [in]  ������� �� �������� ������ ����� 											*/
	);
	int spi_dac_read(
		int			dac_index,		/*!< [in]  ���������� ������ ���������� ��� ��	DAC0..DAC67							*/
		uint8_t		addr,			/*!< [in]  ��������� ����� �������� ���������� �� ����� � �������� ���������� ������	*/
		uint8_t		*data,			/*!< [out] ������ ������������ �� ���													*/
		uint8_t		num,			/*!< [in]  ���������� ���� ������ �� 1 �� 4												*/
		uint32_t	timeout			/*!< [in]  ������� �� �������� ������ �����												*/
	);

#ifdef	__cplusplus
}
#endif

#endif /* STM32F4xx_SPIDAC_BRD_H */
