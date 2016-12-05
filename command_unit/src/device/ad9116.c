/*!  \file 	dac.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "ad9116.h"
#include "stm32f4xx_gpio_brd.h"
#include "stm32f4xx_spidac_brd.h"
#include "stm32f4xx_pwmdac_brd.h"
//#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/


/*types=============================================================================================================*/


/*prototypes========================================================================================================*/
//static void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


/*variables=========================================================================================================*/
static	uint8_t				clock_timer_disabled = 1;		/* флаг указывающий работает или нет тактирующий таймер 1-выключен, 0-включен*/
uint8_t						verbuf = 0;

/*code==============================================================================================================*/


/*=============================================================================================================*/
/*!  \brief конфигурируем интерфейсы последовательный и параралельный для управлениея ЦАПом

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_open
    (
     int dac_index      /*!< [in] индекс цапа 0-3 */
     )
    {    
        /* конфигурирование ног паралелльной шины как выходов */
	    gpio_dac_open(dac_index);
    
        /* включение интерфейса SPI2 */
//	    	    gpio_dac_spi_mode(dac_index);
	    
//	    spi_dac_open(dac_index);    
//	    gpio_dacspi_open(dac_index);
        
        /* включение тактирующего таймера */
//	    if (clock_timer_disabled)
//	    {
//		    clock_timer_disabled = 0;
//		    /* tim3 ch3 */
//		    pwm_dac_open();		    
//		    gpio_dacpwm_open();		    
//		}
//	    
//	    for (;;)
//	    {
//		    spi_dac_read(dac_index, AD9116_VERSION, &verbuf, 1, 1000);
//		    continue;		    
//	    }    
	    
	    gpio_dacspi_close(dac_index);
   	    gpio_dac_pinmode(dac_index);
	    
		return OK;
    }



/*=============================================================================================================*/
/*!  \brief выключаем интерфейсы работаюшие с ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_close
    (
     int dac_index      /*!< [in] индекс цапа 0-3 */
     )
    {    
    /**/
	gpio_dac_close(dac_index);
	gpio_dacspi_close(dac_index);
		    
//    if (!clock_timer_disabled)
//    {
//	    gpio_dacpwm_close();
//	    
//	    clock_timer_disabled = 1;
//    }
//	    
	    
	    
    return OK;    
    }



/*=============================================================================================================*/
/*!  \brief  Запись цифровых данных в ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_write_data
    (
     int                    dac_index,  /*!< [in] индекс цапа 0-3                           */
     enum AD9116_INTERFACE  interface,  /*!< [in] интерфейс по которому передаются данные   */
	 dac_values_t			data		/*!< [in] данные для выхода i и q ЦАПа			    */
	)
    {
	    int		i;
	    /* переводим reset в high если в низу */
			    
	    switch (interface)
	    {
	    case AD9116_PARALLEL:
		    {
			    if (dac_index & DAC0) {
				    gpio_dac0_write(data.i, data.q);
				    gpio_dac0_write(data.i, data.q);
				    gpio_dac0_write(data.i, data.q);
				    gpio_dac0_write(data.i, data.q);
			    } 
			    if (dac_index & DAC1) {
				    gpio_dac1_write(data.i, data.q);
				    gpio_dac1_write(data.i, data.q);
				    gpio_dac1_write(data.i, data.q);
				    gpio_dac1_write(data.i, data.q);
			    } 
			    if (dac_index & DAC2) {
				    gpio_dac2_write(data.i, data.q);
				    gpio_dac2_write(data.i, data.q);
				    gpio_dac2_write(data.i, data.q);
				    gpio_dac2_write(data.i, data.q);
			    } 
			    if (dac_index & DAC3) {
				    gpio_dac3_write(data.i, data.q);
				    gpio_dac3_write(data.i, data.q);
				    gpio_dac3_write(data.i, data.q);
				    gpio_dac3_write(data.i, data.q);
			    }
		    } break;
	    }
	    	    
    return OK;    
    }
    
/*=============================================================================================================*/
/*!  \brief запись последовательно во все ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int     ad9116_write_all_parralel
    (
    int                    dac_index,  /*!< [in] индекс цапа 0-3                           */
	uint16_t               *data,	   /*!< [in] данные для выхода i ЦАПа				    */
	uint8_t                len		   /*!< [in] данные для выхода q ЦАПа    */
	)
{
	
	
	return OK;
}



/*=============================================================================================================*/
/*!  \brief считывание текущих настроек ЦАП

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
int  ad9116_write_register
		(
			int							dac_index,  /*!< [in] индекс цапа 0-3                           */
			uint8_t						adress,
			void						*data
		)
{
	
	spi_dac_write(dac_index, adress, (uint8_t*)data, 1, 1000);
	
	return OK;
}
	
/*=============================================================================================================*/
/*!  \brief считывание текущих настроек ЦАП

     \return 
     \retval 
     \sa 
*/
/*=============================================================================================================*/
int  ad9116_read_register
	(
		int						dac_index,  /*!< [in] индекс цапа 0-3                           */
		uint8_t					adress,
		void					*data
	)
{
	uint8_t	read_temp;
	
	if (data == NULL) {
		return ERR;
	}
	
	spi_dac_read(dac_index, adress, &read_temp, 1, 1000);
	
	*(uint8_t*)data = read_temp;
		
	return OK;
}
		



/*=============================================================================================================*/
/*!  \brief Чтение данных с ЦАП

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
uint8_t     ad9116_read_id
    (
     int                    dac_index   /*!< [in] индекс цапа 0-3                           */
     )
    {
	    uint8_t		id = 0;
    
	    for (;;)
	    {
		    spi_dac_read(DAC0, AD9116_VERSION_ADDR, &id, 1, 1000);		    
	    }
	    
		return id;    
    }
    

