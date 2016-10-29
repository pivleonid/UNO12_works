/*!  \file 	cpld_bus.c
 *   \brief  
 *   \details  
 *
 *
 *
 */

/*includes==========================================================================================================*/
#include "cpld_bus.h"
#include <stm32f4xx_hal.h>

/*defines===========================================================================================================*/

#define MODE_WRITE      (0)
#define MODE_READ       (1)

/*types=============================================================================================================*/

/*prototypes========================================================================================================*/

/*variables=========================================================================================================*/
static uint8_t     mode;

/*code==============================================================================================================*/
/*=============================================================================================================*/
/*!  \brief Конфигурирует GPIO для работы с CPLD (дешифратором управления АТ)

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int cpld_open( void )
{
	GPIO_InitTypeDef    GPIO_InitStructure;

    __GPIOD_CLK_ENABLE();

    /* линии данных */
    GPIO_InitStructure.Pin =  CPLD_D0 | CPLD_D1 | CPLD_D2 | CPLD_D3 
                            | CPLD_D4 | CPLD_D5 | CPLD_D6 | CPLD_D7;

    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
    
    /* линии стробов */
    GPIO_InitStructure.Pin =  CPLD_WR | CPLD_RD | CPLD_CS0 | CPLD_CS1; 
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
    
    mode = MODE_WRITE;
    return OK;
}


/*=============================================================================================================*/
/*!  \brief Выключает порт работающий с CPLD (дешифратором управления АТ)

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int cpld_close( void )
{
    GPIO_InitTypeDef    GPIO_InitStructure;

    GPIO_InitStructure.Pin =  CPLD_D0 | CPLD_D1 | CPLD_D2 | CPLD_D3 
                            | CPLD_D4 | CPLD_D5 | CPLD_D6 | CPLD_D7;

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
    
    /* линии стробов */
    GPIO_InitStructure.Pin =  CPLD_WR | CPLD_RD | CPLD_CS0 | CPLD_CS1; 
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
    
    mode = MODE_READ;
    __GPIOD_CLK_DISABLE();
    return OK;
}



/*=============================================================================================================*/
/*!  \brief  Чтение из CPLD (дешифратора управления АТ) кода состояния элемента управления АТ (пока не поддерживается)

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int cpld_read
(
    uint8_t    code,   /*!< [in]  ID элемента АТ         */
    uint8_t    *data,  /*!< [out] указатель на переменную для считывания кода сотояния АТ */
    int        cs      /*!< [in]  выбор дешифратора */
 )
{
    uint16_t            cs_pin;
    GPIO_InitTypeDef    GPIO_InitStructure;
    
    
    if ( mode != MODE_WRITE ) {
                                                                                                /* переставляем пины в состоянии выходов */    
        GPIO_InitStructure.Pin =  CPLD_D0 | CPLD_D1 | CPLD_D2 | CPLD_D3 
                                | CPLD_D4 | CPLD_D5 | CPLD_D6 | CPLD_D7;

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
        mode = MODE_WRITE;
      
        }
    
    switch ( cs ) {
                                                                                               /* выюор CPLD */
        case CPLD_SELECT_CS0:  cs_pin = CPLD_CS0; break;
        case CPLD_SELECT_CS1:  cs_pin = CPLD_CS1; break;
        default             :  cs_pin = CPLD_CS0; break;
    }
    
                                                                                                /* запись кода элемента AT */
    HAL_GPIO_WritePin( GPIOD, CPLD_WR | CPLD_RD | CPLD_CS0 | CPLD_CS1, GPIO_PIN_SET );
    
    HAL_GPIO_WritePin( GPIOD, cs_pin, GPIO_PIN_RESET );

    HAL_GPIO_WritePin( GPIOD, CPLD_WR, GPIO_PIN_RESET );    
    HAL_GPIO_WritePort( GPIOD, code, 8 );
    HAL_GPIO_WritePin( GPIOD, CPLD_WR, GPIO_PIN_SET );    
    
                                                                                               /* считывание состояния элемента AT */
    GPIO_InitStructure.Pin =  CPLD_D0 | CPLD_D1 | CPLD_D2 | CPLD_D3 
                            | CPLD_D4 | CPLD_D5 | CPLD_D6 | CPLD_D7;

    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
    mode = MODE_READ;

    HAL_GPIO_WritePin( GPIOD, CPLD_RD, GPIO_PIN_RESET );    
//    HAL_GPIO_ReadPort( GPIOD, data, 8 );
    HAL_GPIO_WritePin( GPIOD, CPLD_RD, GPIO_PIN_SET );    
    
    HAL_GPIO_WritePin( GPIOD, cs_pin, GPIO_PIN_SET );
    
    return OK;
}


/*=============================================================================================================*/
/*!  \brief Запись в CPLD (дешифратора управления АТ) кода состояния элемента управления АТ 

     \return int
     \retval OK, ERROR
     \sa 
*/
/*=============================================================================================================*/
int cpld_write
(
    uint8_t    code,       /*!< [in]  ID элемента АТ         */
    uint8_t    data,        /*!< [in]  код сотояния АТ */
    int        cs           /*!< [in]  выбор дешифратора */
 )
    {
    uint16_t        cs_pin;
    
    if ( mode != MODE_WRITE ) {
        GPIO_InitTypeDef    GPIO_InitStructure;
        
        GPIO_InitStructure.Pin =  CPLD_D0 | CPLD_D1 | CPLD_D2 | CPLD_D3 
                                | CPLD_D4 | CPLD_D5 | CPLD_D6 | CPLD_D7;

        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
        GPIO_InitStructure.Pull = GPIO_NOPULL;
        HAL_GPIO_Init( GPIOD, &GPIO_InitStructure );
        mode = MODE_WRITE;      
        }
    
    switch ( cs )
        {
            case CPLD_SELECT_CS0:  cs_pin = CPLD_CS0;   break;
            case CPLD_SELECT_CS1:  cs_pin = CPLD_CS1;   break;
            default             :  cs_pin = CPLD_CS0;   break;
        }
    
    HAL_GPIO_WritePin( GPIOD, CPLD_WR | CPLD_RD | CPLD_CS0 | CPLD_CS1, GPIO_PIN_SET );
    
    HAL_GPIO_WritePin( GPIOD, cs_pin, GPIO_PIN_RESET);

    HAL_GPIO_WritePin( GPIOD, CPLD_WR, GPIO_PIN_RESET );    
    HAL_GPIO_WritePort( GPIOD, code, 8 );
    HAL_GPIO_WritePin( GPIOD, CPLD_WR, GPIO_PIN_SET );    
    
    HAL_GPIO_WritePin( GPIOD, CPLD_WR, GPIO_PIN_RESET );    
    HAL_GPIO_WritePort( GPIOD, data, 8 );
    HAL_GPIO_WritePin( GPIOD, CPLD_WR, GPIO_PIN_SET );    
    
    HAL_GPIO_WritePin( GPIOD, cs_pin, GPIO_PIN_SET );
    
    return OK;
}
