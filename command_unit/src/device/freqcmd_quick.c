/*!  \file 	cpld_bus.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "freqcmd_quick.h"
#include "cpld.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/


/*variables=========================================================================================================*/


/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief ������������� ��� �������� �� � ���������� � ������� �������� �������� �������

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
void freqcmd 
(
 uint16_t   freq        /*!< [in] ������� ������� �� ������ ���������� ��������� �� */
)
{
    uint8_t         code;       /* ID �������� ��         */
    uint8_t         data;       /* ��� ��������� �������� */
    uint8_t         cs;         /* ����� �����������      */

    /*  */
//    code = tabl[freq_index].code;
    

    /* ������ � ���� cpld ���� � ������ */
//    if ( cs == CPLD_SELECT_CS0 )    {
//        GPIOD->BSRR = CPLD_CS0;    
//    } else {
//        GPIOD->BSRR = CPLD_CS1;        
//    }
//    GPIOD->BSRR = CPLD_WR;    
//    GPIOD->BSRR = (((uint32_t)(~code)) << 16)  || code;
//    GPIOD->BSRR = (uint32_t)CPLD_WR << 16;    
//
//    GPIOD->BSRR = CPLD_WR;    
//    GPIOD->BSRR = (((uint32_t)(~data)) << 16)  || data;
//    GPIOD->BSRR = (uint32_t)CPLD_WR << 16;    
//
//    if ( cs == CPLD_SELECT_CS0 ) {
//        GPIOD->BSRR = (uint32_t)CPLD_CS0 << 16;    
//    }
//    else {
//        GPIOD->BSRR = (uint32_t)CPLD_CS1 << 16;        
//    }
//



}