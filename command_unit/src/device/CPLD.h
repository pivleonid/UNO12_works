#ifndef CPLD_H
#define CPLD_H	

#ifdef	__cplusplus
extern "C" {
#endif

/*!  \file 	CPLD_Preselector.h
*   \brief  ���� ������� ���������� ����� ������������ � ����������, ������������� ���������
*   CPLD, ������� �� �������.
*	cpld_open	 -> ������������ ��������������� �����
*   cpld_close   -> �������� �����
*   cpld_write   -> ��������� �������� CPLD
*
*/

/*includes==========================================================================================================*/
#include <math.h>
#include <stdint.h>
/*defines===========================================================================================================*/
#define		CPLD_ID_UNOMULT		(0x34u)						/*!<	*/
#define CPLD_CODE_UNOMULT_X1MXLow			(0x00u)		/*!<	*/
#define CPLD_CODE_UNOMULT_X2MXLow			(0x01u)		/*!<	*/
#define CPLD_CODE_UNOMULT_X2MXHigh			(0x02u)		/*!<	*/
#define CPLD_CODE_UNOMULT_X4MXHigh			(0x03u)		/*!<	*/

#define CPLD_MSHU_ID			(0x01u)
#define CPLD_MSHUPRES_ID		(0x02u)
#define CPLD_MECHATT_ID			(0x03u)
#define CPLD_RF_TEST_ID			(0x04u)
#define CPLD_PHILTERES_ID		(0x05u)
#define CPLD_DIGATT_ID			(0x06u)
#define CPLD_PRES_ONOFF_ID		(0x07u)
#define CPLD_L_H_BAND_ID		(0x08u)

/*types=============================================================================================================*/
/*prototypes========================================================================================================*/
void cpld_open(void);
void cpld_close(void);
void cpld_write(
				uint8_t AT_ID,/*!< [in]  ID �������� ����������� ������    								*/
				uint8_t number/*!< [in]  ��� �� ����� ��������(CPLD � ������ ������)					*/
				);


#ifdef	__cplusplus
}
#endif

#endif /* !CPLD.h */