/*!  \file 	dac.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "dac.h"
#include "ad9116.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/


/*variables=========================================================================================================*/
static dac_values_t dac_buffer[4] = 
{ 
	{ 0x800, 0x800 },
	{ 0x800, 0x800 },
	{ 0x800, 0x800 },
	{ 0x800, 0x800 },			
};


/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     dac_open
    (
		cu_type_sts_t	type
    )
{
	ad9116_idacgain_t	dacgain;	
	uint8_t				version = 0;
	uint8_t				spiconf;
	
	/* конфигурируем интерфейсы ЦАПов */
	if (CU_TYPE_HETERODIN == type)
	{
		ad9116_open(DAC_ALL);
//		ad9116_open(DAC_23);
//		ad9116_open(DAC_45);	
//		ad9116_open(DAC_67);	
//	/* конфигурируем шкалу цап, настраиваем */
//	for (;;)
	{
//		dacgain.gain = 33;
//		ad9116_write_register(DAC_01, AD9116_IDACGAIN_ADDR, &dacgain);
//		
//		dacgain.gain = 0;
//		ad9116_read_register(DAC_01, AD9116_IDACGAIN_ADDR, &dacgain);
//		
////		spiconf = 0;
//		ad9116_read_register(DAC_01, AD9116_SPICONTROL_ADDR, &spiconf);
//		
//		version = 0;
//		ad9116_read_register(DAC_01, AD9116_VERSION_ADDR, &version);
//		ad9116_read_register(DAC_01, AD9116_VERSION_ADDR, &version);
		
	//	continue;
	}
//	} else {
		;
	}
    
//	for (;;)
//	{
//		static uint16_t i  = 0;
		
//		dac_buffer[0].i = dac_buffer[0].q = i & 0x0FFF;
//		dac_buffer[1].i = dac_buffer[1].q = i & 0x0FFF;
//		dac_buffer[2].i = dac_buffer[2].q = i & 0x0FFF;
//		dac_buffer[3].i = dac_buffer[3].q = i & 0x0FFF;
	ad9116_write_data(DAC_01, AD9116_PARALLEL, dac_buffer[0]);		
	ad9116_write_data(DAC_23, AD9116_PARALLEL, dac_buffer[1]);	
	ad9116_write_data(DAC_45, AD9116_PARALLEL, dac_buffer[2]);			
	ad9116_write_data(DAC_67, AD9116_PARALLEL, dac_buffer[3]);			
		
//		HAL_Delay(100);
//	
//		i = ~i;		
//	}
//	
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     dac_close 
    (
		cu_type_sts_t	type
    )
{
	
	
	return OK;
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     dac_write
    (
		uint8_t		index,	/*!< [in] индекс цапа относительно БУ 0-7		*/
		uint16_t	code	/*!< [in] код пишушийся в соответвующий индекс	*/	
	)
{	
	int		hrdwr_dac_indx = DAC_01;
	
	switch (index) {
		case 0:
		case 1:	hrdwr_dac_indx = DAC_01;	break;
		
		case 2:
		case 3: hrdwr_dac_indx = DAC_23;	break;
		
		case 4:
		case 5: hrdwr_dac_indx = DAC_45;	break;
		
		case 6:
		case 7: hrdwr_dac_indx = DAC_67;	break;
	}
	
	if ((index % 2) > 0){
		dac_buffer[index / 2].i = code;		
	} else	{
		dac_buffer[index / 2].q = code;				
	}
	
	ad9116_write_data(hrdwr_dac_indx, AD9116_PARALLEL, dac_buffer[index / 2]);
	
	return OK;	
}


/*=============================================================================================================*/
/*!  \brief 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     dac_read
    (
		uint8_t		index,	/*!< [in] индекс цапа относительно БУ 0-7		*/
		uint16_t	*code	/*!< [in] код пишушийся в соответвующий индекс	*/
	)
{
	if (code == NULL) {
		return ERR;
	}
	
	if ((index % 2) > 0) {
		*code = dac_buffer[index / 2].i;		
	}
	else {
		*code = dac_buffer[index / 2].q;				
	}
	
	return OK;	
}



