#ifndef CPLD_H
#define CPLD_H	

#include <math.h>
#include <stdint.h>

#define		CPLD_ID_UNOMULT		(0x34u)						/*!<	*/
	#define CPLD_CODE_UNOMULT_X1MXLow			(0x00u)		/*!<	*/
	#define CPLD_CODE_UNOMULT_X2MXLow			(0x01u)		/*!<	*/
	#define CPLD_CODE_UNOMULT_X2MXHigh			(0x02u)		/*!<	*/
	#define CPLD_CODE_UNOMULT_X4MXHigh			(0x03u)		/*!<	*/


void cpld_open(void);
void cpld_close(void);
void cpld_write(uint8_t id, uint8_t code);

#endif /* !CPLD.h */


    
	
	
