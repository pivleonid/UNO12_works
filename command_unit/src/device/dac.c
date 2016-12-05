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
enum _DACOUTS
{
	dacout_i,
	dacout_q
};

enum _DACALIVE
{
	dacalive_ok,
	dacalive_not
};

/*prototypes========================================================================================================*/


/*variables=========================================================================================================*/
static dac_values_t dac_buffer[4] = 
{ 
	{ 0x800, 0x800 },						/* DAC0 */
	{ 0x800, 0x800 },						/* DAC1 */
	{ 0x800, 0x800 },						/* DAC2 */
	{ 0x800, 0x800 },						/* DAC3 */
};

static struct _DAC_ADDRAY
{
	int				dac_mask;	
	enum _DACOUTS	out;
	enum _DACALIVE	status;
	dac_values_t	*values;
} hrdwr_dac_indx[] = 
{ 
	{ DAC0, dacout_q, dacalive_ok, &dac_buffer[0]},		//DAC0+ DAC0-
	{ DAC1, dacout_q, dacalive_ok, &dac_buffer[1]},		//DAC1+ DAC1-
	{ DAC2, dacout_q, dacalive_ok, &dac_buffer[2]},		//DAC2+ DAC2-
	{ DAC0, dacout_i, dacalive_ok, &dac_buffer[0]},		//DAC3+ DAC3-
	{ DAC1, dacout_i, dacalive_ok, &dac_buffer[1]},		//DAC4+ DAC4-
	{ DAC2, dacout_i, dacalive_ok, &dac_buffer[2]},		//DAC5+ DAC5-
	{ DAC3, dacout_q, dacalive_ok, &dac_buffer[3]},		//DAC6+ DAC6-
	{ DAC3, dacout_i, dacalive_ok, &dac_buffer[3]},		//DAC7+ DAC7-
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

	ad9116_open(DAC_ALL);
	
		// TODO  ad9116_ioctl(MODE, SPIMODE);
		
		/* получем id со всех ЦАП */
//		version = 0;
//		ad9116_read_register(DAC0, AD9116_VERSION_ADDR, &version);
//		ad9116_read_register(DAC0, AD9116_VERSION_ADDR, &version);
	;
	/* если не получили возвращаем код ошибки  */
	;
	/* конфигурируем цап */
	/* IFIRST = 0, IRISING = 1 */
	;

			// TODO  ad9116_ioctl(MODE, PINMODE);
	;

#ifdef	DAC_TEST
	for (;;)
	{
		static int16_t i  = 0;
		static int16_t add = 0x0333;
		
		dac_buffer[0].i = ~(dac_buffer[0].q = i & 0x0FFF) & 0x0FFF;
		dac_buffer[1].i = ~(dac_buffer[1].q = i & 0x0FFF) & 0x0FFF;
		dac_buffer[2].i = ~(dac_buffer[2].q = i & 0x0FFF) & 0x0FFF;
		dac_buffer[3].i = ~(dac_buffer[3].q = i & 0x0FFF) & 0x0FFF;
#endif	
	
	if (CU_TYPE_HETERODIN == type)
	{
		/* выставляем уровень по умолчанию  */
		ad9116_write_data(DAC0, AD9116_PARALLEL, dac_buffer[0]);		
		ad9116_write_data(DAC1, AD9116_PARALLEL, dac_buffer[1]);	
		ad9116_write_data(DAC2, AD9116_PARALLEL, dac_buffer[2]);			
		ad9116_write_data(DAC3, AD9116_PARALLEL, dac_buffer[3]);			
		
	} else 
	if (CU_TYPE_PRESELECTOR == type) {
		ad9116_write_data(DAC0, AD9116_PARALLEL, dac_buffer[0]);		
		ad9116_write_data(DAC1, AD9116_PARALLEL, dac_buffer[1]);	
		ad9116_write_data(DAC2, AD9116_PARALLEL, dac_buffer[2]);			
		ad9116_write_data(DAC3, AD9116_PARALLEL, dac_buffer[3]);			
	}
    
#ifdef DAC_TEST
		HAL_Delay(5);
	
	//	i = ~i;		
		i += add;
		if ( (i + add) & 0xF000)
//		{	add = -add;	}
		{	i = 0;	}
		
		
	}				  
#endif // DAC_TEST
		
	
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
int dac_write
    (
		uint8_t		index,	/*!< [in] индекс выхода цапа относительно БУ 0-7	*/
		uint16_t	code	/*!< [in] код пишушийся в соответвующий выход		*/	
	)
{		
	switch (hrdwr_dac_indx[index].out)
	{		
	case dacout_i:	hrdwr_dac_indx[index].values->i = code & 0xFFF; break;
	case dacout_q:	hrdwr_dac_indx[index].values->q = code & 0xFFF; break;
	}
	
	ad9116_write_data(hrdwr_dac_indx[index].dac_mask, AD9116_PARALLEL, *hrdwr_dac_indx[index].values);
	
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
	
	switch (hrdwr_dac_indx[index].out)
	{		
	case dacout_i:	*code = hrdwr_dac_indx[index].values->i; break;
	case dacout_q:	*code = hrdwr_dac_indx[index].values->q; break;
	}
		
	
	return OK;	
}



