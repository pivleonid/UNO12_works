#ifndef CPLD_H
#define CPLD_H	

#include <math.h>
#include <stdint.h>

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


void cpld_open(void);
void cpld_close(void);
void cpld_write(uint8_t, uint8_t);

#endif /* !CPLD.h */