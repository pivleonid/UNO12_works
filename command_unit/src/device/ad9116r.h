#ifndef		AD9116R_H
#define		AD9116R_H

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
#define     AD9116_SPICONTROL_ADDR     (0x00u)
    #define     AD9116_SPICONTROL_LSBFIRST          (0x1u << 6)
    #define     AD9116_SPICONTROL_LNGINS            (0x1u << 4)

#define     AD9116_POWERDOWN_ADDR      (0x01u)
    #define     AD9116_POWERDOWN_LDOOFF             (0x1u << 7)
    #define     AD9116_POWERDOWN_LDOSTAT            (0x1u << 6)
    #define     AD9116_POWERDOWN_PWRDN              (0x1u << 5)
    #define     AD9116_POWERDOWN_QDACOFF            (0x1u << 4)
    #define     AD9116_POWERDOWN_IDACOFF            (0x1u << 3)
    #define     AD9116_POWERDOWN_QCLKOFF            (0x1u << 2)
    #define     AD9116_POWERDOWN_ICLKOFF            (0x1u << 1)
    #define     AD9116_POWERDOWN_EXTREF             (0x1u << 0)
        
#define     AD9116_DATACONTROL_ADDR     (0x02u)
    #define     AD9116_DATACONTROL_TWOS             (0x1 << 7)
    #define     AD9116_DATACONTROL_IFIRST           (0x1 << 5)
    #define     AD9116_DATACONTROL_IRISING          (0x1 << 4)
    #define     AD9116_DATACONTROL_SIMULBIT         (0x1 << 3)
    #define     AD9116_DATACONTROL_DCI_EN           (0x1 << 2)
    #define     AD9116_DATACONTROL_DCOSGL           (0x1 << 1)
    #define     AD9116_DATACONTROL_DCODBL           (0x1 << 0)
    
#define     AD9116_IDACGAIN_ADDR         (0x03u)
    #define     AD9116_IDACGAIN_VAL(a)              (((a) & 0x3F) << 0)
    
#define     AD9116_IRSET_ADDR            (0x04u)
    #define     AD9116_IRSET_IRSETEN                (0x1 << 7)
    #define     AD9116_IRSET_VAL(a)                 (((a) & 0x3F) << 0)
        
#define     AD9116_IRCML_ADDR            (0x05u)
    #define     AD9116_IRCML_IRCMLEN                (0x1 << 7)
    #define     AD9116_IRCML_VAL(a)                 (((a) & 0x3F) << 0)

#define     AD9116_QDACGAIN_ADDR         (0x06u)
    #define     AD9116_QDACGAIN_VAL(a)              (((a) & 0x3F) << 0)
    
#define     AD9116_QRSET_ADDR            (0x07u)
    #define     AD9116_QRSET_QRSETEN                (0x1 << 7)
    #define     AD9116_QRSET_VAL(a)                 (((a) & 0x3F) << 0)
        
#define     AD9116_QIRCML_ADDR            (0x08u)
    #define     AD9116_QRCML_QRCMLEN                (0x1 << 7)
    #define     AD9116_QRCML_VAL(a)                 (((a) & 0x3F) << 0)
        
#define     AD9116_AUXDAC_Q_ADDR          (0x09u)
    #define     AD9116_AUXDACQ_VAL(a)               (((a) & 0xFF) << 0)
    
#define     AD9116_AUXCTLQ_ADDR           (0x0Au)
    #define     AD9116_AUXCTLQ_QAUXDAC(a)           (((a) & 0x03) << 0)
    #define     AD9116_AUXCTLQ_QAUXOFS(a)           (((a) & 0x07) << 2)
    #define     AD9116_AUXCTLQ_QAUXRNG(a)           (((a) & 0x03) << 5)
    #define     AD9116_AUXCTLQ_QAUXEN               (1 << 7)
    
#define     AD9116_AUXDAC_I_ADDR          (0x0Bu)
#define     AD9116_AUXDACI_VAL(a)               (((a) & 0xFF) << 0)
    
#define     AD9116_AUXCTLI_ADDR           (0x0Cu)
    #define     AD9116_AUXCTLI_IAUXDAC(a)           (((a) & 0x03) << 0)
    #define     AD9116_AUXCTLI_IAUXOFS(a)           (((a) & 0x07) << 2)
    #define     AD9116_AUXCTLI_IAUXRNG(a)           (((a) & 0x03) << 5)
    #define     AD9116_AUXCTLI_IAUXEN               (1 << 7)
    
#define     AD9116_REFRES_ADDR           (0x0Du)
    #define     AD9116_REFRES_VAL(a)                (((a) & 0x3F) << 0)
    
#define     AD9116_CALCONTROL_ADDR        (0x0Eu)
    #define     AD9116_CALCONTROL_DIVSEL(a)         (((a) & 0x03) << 0)
    #define     AD9116_CALCONTROL_CALCLK            (1 << 3)
    #define     AD9116_CALCONTROL_CALSELI           (1 << 4)
    #define     AD9116_CALCONTROL_CALSELQ           (1 << 5)
    #define     AD9116_CALCONTROL_PRELDI            (1 << 6)
    #define     AD9116_CALCONTROL_PRELDQ            (1 << 7)
    
#define     AD9116_CALMEMORY_ADDR        (0x0Fu)
    #define     AD9116_CALMEMORY_CALMEMI(a)         (((a) & 0x03) << 0)
    #define     AD9116_CALMEMORY_CALMEMQ(a)         (((a) & 0x03) << 2)
    #define     AD9116_CALMEMORY_CALSTATI           (1 << 6)
    #define     AD9116_CALMEMORY_CALSTATQ           (1 << 7)
    
#define     AD9116_MEMORY_ADRESS_ADDR    (0x10u)
    #define     AD9116_MEMORY_ADRESS_VAL(a)         (((a) & 0x3F) << 0)

#define     AD9116_MEMORY_DATA_ADDR      (0x11u)
    #define     AD9116_MEMORY_DATA_VAL(a)           (((a) & 0x3F) << 0)

#define     AD9116_MEMORY_RW_ADDR      (0x12u)
    #define     AD9116_MEMORY_RW_UNCALI             (1 << 0)
    #define     AD9116_MEMORY_RW_UNCALQ             (1 << 1)
    #define     AD9116_MEMORY_RW_SMEMRD             (1 << 2)
    #define     AD9116_MEMORY_RW_SMEMRW             (1 << 3)
    #define     AD9116_MEMORY_RW_CALEN              (1 << 4)
    #define     AD9116_MEMORY_RW_CALRSTI            (1 << 6)
    #define     AD9116_MEMORY_RW_CALRSTQ            (1 << 7)
    
#define     AD9116_CLKMODE_ADDR       (0x14u)
    #define     AD9116_CLKMODE_I(a)                 (((a) & 0x03) << 0)
    #define     AD9116_CLKMODE_EN                   (1 << 2)
    #define     AD9116_CLKMODE_REACQUIRE            (1 << 3)
    #define     AD9116_CLKMODE_SEARCHING            (1 << 4)

#define     AD9116_VERSION_ADDR       (0x1Fu)

    
	
/*types=============================================================================================================*/
	typedef struct _AD9116_SPICONTROL_T
	{
		uint8_t		reserved : 4;
		uint8_t		lngins   : 1;
		uint8_t		reset    : 1;
		uint8_t		lsbfirst : 1;
	}ad9116_spicontrol_t;

	typedef struct _AD9116_IDACGAIN_T
	{
		uint8_t		gain : 6;
		uint8_t		reserved : 2;		
	}ad9116_idacgain_t;
	
	
	
/*prototypes========================================================================================================*/

 
#ifdef	__cplusplus
	}
#endif

#endif /* AD9116R_H */

