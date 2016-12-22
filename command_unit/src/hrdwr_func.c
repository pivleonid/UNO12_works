/* Sep 16, 2015 */

/*!  \file 	hrdwr_func.c
 *   \brief  функции для обработчиков команд и хранение текущих значений аппаратных коэффициэнтов
 *   \details  
 */

/*includes==========================================================================================================*/
#include "hrdwr_func.h"
#include "device/dac.h"
#include "device/InitializationUNO_v1.h"
#include "device/CPLD.h"

/*defines===========================================================================================================*/


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/


/*variables=========================================================================================================*/

//static uint8_t      id_now;
//static uint8_t      code_now;
static struct  _BU_ALL
{
	uint16_t     daccode[8];
} bu = {
	{ 0x0010, 0x0020, 0x0030, 0x0040, 0x0050, 0x0060, 0x0070, 0x0080 },
};


static struct _HETERODINE
{
	het_flags_t	 multipler;		
	uint8_t      dds_gain;
	uint64_t     dds_freq;	
} heterodine[2] = {
	{HETMUL_OFF, 0, 0},
	{HETMUL_OFF, 0, 0},
};

static struct _PRESELECTOR
{
	uint8_t			mech_att_gain;
	att_flags_t		flag_att;
} pres = { 65, { 1, 0, 1, 0 } };
	
static uint8_t			yig = 0;
static cu_type_sts_t	cu_type = CU_TYPE_HETERODIN;

/*code==============================================================================================================*/


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
cu_type_sts_t get_cu_type(void)
{
	return cu_type;
}

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_cu_type(cu_type_sts_t type)
{
	cu_type = type;
}

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_mechatt_gain( uint8_t gain )
    {
	    if (pres.mech_att_gain != gain)
	    {
		    /*<0-65 с шагом 5 дб>*/
		    cpld_write(CPLD_MECHATT_ID, gain / 5);
		    pres.mech_att_gain = gain;		    
	    }
    }

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint8_t get_mechatt_gain( void )
    {
        return pres.mech_att_gain;
    }

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_mechatt_flags( const struct  _ATT_FLAGS *flags )
    {
	    struct  _ATT_FLAGS temp = *flags;

	    if (temp.lna != pres.flag_att.bitflags.lna) {
		    cpld_write(CPLD_MSHU_ID, (uint8_t)(temp.lna));		    
	    }
	    
	    if (temp.preslna != pres.flag_att.bitflags.preslna) {
		    cpld_write(CPLD_MSHUPRES_ID, (uint8_t)(temp.preslna));
	    }
	    
	    if (temp.on != pres.flag_att.bitflags.on) {
		    cpld_write(CPLD_PRES_ONOFF_ID, (uint8_t)(temp.on));		    
	    }
	    
	    if (temp.test != pres.flag_att.bitflags.test) {
		    cpld_write(CPLD_RF_TEST_ID, (uint8_t)(temp.test));
	    }
	    
	    pres.flag_att.bitflags = temp;
    }

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint8_t get_mechatt_flags( void )
    {
        return pres.flag_att.byteflags;
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_decoder_code( uint8_t  id, uint8_t code )
    {
	    cpld_write(id, code);
//        id_now = id;
//        code_now = code;
    }

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_dac_code( uint8_t  id, uint16_t code )
    {
	    if (dac_write(id, code) == OK) {
		    bu.daccode[id] = code;		    
	    }
    }

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint16_t get_dac_code(uint8_t  id)
{
	uint16_t	code;
	
	if (dac_read(id, &code) == OK) {
		bu.daccode[id] = code;
	}
	
	return bu.daccode[id];
}


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_dds_code( uint8_t  id, uint32_t freq, uint8_t gain )
    {
	    heterodine[id].dds_gain = gain;
	    heterodine[id].dds_freq = freq;
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_yig_sts( uint8_t on )
    {
        yig = on;
    }    
    
/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint8_t     get_yig_sts( void )
    {
        return yig;
    }


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint8_t	get_het_mul_sts(int het_index)
{
	return heterodine[het_index].multipler.byteflags;
}

/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint8_t	get_het_gain_sts(int het_index)
{
	return heterodine[het_index].dds_gain;
}


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
uint32_t get_het_freq_sts(int het_index)  
{
	return heterodine[het_index].dds_freq;
}


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_het_mul(uint8_t code)
{
	if (heterodine[0].multipler.bitflags.mulstate != (code & 0x3))
	{
		heterodine[0].multipler.byteflags = heterodine[1].multipler.byteflags = code & 0x3;	
		
		/* передача кода в cpld */
		switch (heterodine[0].multipler.bitflags.mulstate)
		{
		case HETMUL_OFF:		cpld_write(CPLD_ID_UNOMULT, CPLD_CODE_UNOMULT_X1MXLow); break;
		case HETMUL_X2LOW:		cpld_write(CPLD_ID_UNOMULT, CPLD_CODE_UNOMULT_X2MXLow); break;
		case HETMUL_X2HIGH:		cpld_write(CPLD_ID_UNOMULT, CPLD_CODE_UNOMULT_X2MXHigh); break;
		case HETMUL_X4HIGH:		cpld_write(CPLD_ID_UNOMULT, CPLD_CODE_UNOMULT_X4MXHigh); break;
		}
	}
}


/*=============================================================================================================*/
/*!  \brief

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
void set_het(
	int het_index, 
	uint8_t gain, 
	uint64_t freq		/*!< [in] freq = 0 – 130000000 (0 -13 Ггц с шагом 0.1 Гц) */
	)
{	
	
	if (uno_write(het_index, ((float) freq)/10000000.0, gain) == OK)
	{
		heterodine[het_index].dds_gain = gain;	
		heterodine[het_index].dds_freq = gain;			
	}
}


