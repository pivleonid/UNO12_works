#include "CPLD.h"
#include <stm32f4xx_hal.h>
	#define ADDR1 GPIO_PIN_15
	#define ADDR0 GPIO_PIN_14
	#define ADDR2 GPIO_PIN_13
	#define STROB GPIO_PIN_12
	 //--//
	 #define data0 GPIO_PIN_0
	 #define data1 GPIO_PIN_1
	 #define data2 GPIO_PIN_2
	 #define data3 GPIO_PIN_3
	 #define data4 GPIO_PIN_4
	 #define data5 GPIO_PIN_5
	 #define data6 GPIO_PIN_6
	 #define data7 GPIO_PIN_7

static void Perevod(uint8_t numb); 
static void func1(void);          
static void func2(void);
static void func3(void);

	 void cpld_open(void)
	 {
		 //инициализаци€ портов CPLD
		 //OK Error
	 GPIO_InitTypeDef GPIO_InitStruct;
	 __HAL_RCC_GPIOD_CLK_ENABLE();
     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);
		  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
				    	  |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
						  |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
		  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	 }
    void cpld_close(void)
	 {
		 //выключение портов
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15 
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;   
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	 }
	 
	 void cpld_write(uint8_t id, uint8_t code) //id 0x34;
	 {
		 if (id == 0x34){
			 switch (code) {
			 case 0: //00001011
				func1();
				Perevod(0x0B);
				func2();
				Perevod(0x00);
				func2();
				Perevod(0x00);
				func3();
				break;
			case 1:
				func1();
				Perevod(0x67);
				func2();
				Perevod(0x00);
				func2();
				Perevod(0x00);
				func3();
				break;
			case 2:
				func1();
				Perevod(0xB5);
				func2();
				Perevod(0x01);
				func2();
				Perevod(0x00);
				func3();
				break;
			case 3:
				func1();
				Perevod(0x25);
				func2();
				Perevod(0x01);
				func2();
				Perevod(0x00);
				func3();
				break;  
			default:
					
				break;                
				}
		 }
	 }


//--перевод 1 байта в установку соответствующих ножек --//
//-- ƒл€ CPLD гетеродина --//
static void Perevod(uint8_t numb)
    {

    uint8_t numb_0 = numb & 0x01;
    switch ( numb_0 )
        {
            case 0: HAL_GPIO_WritePin( GPIOD, data7, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data7, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_1 = numb & 0x02;
    switch ( numb_1 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data6, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data6, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_2 = numb & 0x04;
    switch ( numb_2 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data5, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data5, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_3 = numb & 0x08;
    switch ( numb_3 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data4, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data4, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_4 = numb & 0x10;
    switch ( numb_4 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data3, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data3, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_5 = numb & 0x20;
    switch ( numb_5 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data2, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data2, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_6 = numb & 0x40;
    switch ( numb_6 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data1, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data1, GPIO_PIN_SET );
                break;
        }
    uint8_t numb_7 = numb & 0x80;
    switch ( numb_7 ) {
            case 0: HAL_GPIO_WritePin( GPIOD, data0, GPIO_PIN_RESET );
                break;
            default: HAL_GPIO_WritePin( GPIOD, data0, GPIO_PIN_SET );
                break;
        }
   
    }
//--настройки CPLD дл€ гетеродина--//  
static void func1( void )
    {
    HAL_GPIO_WritePin( GPIOD, data0 | data1 | data2 | data3 | data5 | data4 | data6 | data7, GPIO_PIN_RESET );
    HAL_GPIO_WritePin( GPIOD, ADDR0 | ADDR1 | ADDR2 | STROB, GPIO_PIN_RESET );
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, ADDR0, GPIO_PIN_SET );
    }
            
static void func2( void )
    {
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, STROB, GPIO_PIN_SET );
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, STROB, GPIO_PIN_RESET );
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, ADDR0, GPIO_PIN_RESET );
                
    HAL_GPIO_WritePin( GPIOD, ADDR1, GPIO_PIN_SET );
    }

static void func3(void)
    {
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, STROB, GPIO_PIN_SET );
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, STROB, GPIO_PIN_RESET );
    HAL_Delay( 5 );
    HAL_GPIO_WritePin( GPIOD, ADDR0 | ADDR1 | ADDR2, GPIO_PIN_SET );
    HAL_Delay( 5 );
    }
	
	
#undef ADDR1
#undef ADDR0 
#undef ADDR2 
#undef STROB 
#undef data0 
#undef data1 
#undef data2 
#undef data3 
#undef data4 
#undef data5 
#undef data6 
#undef data7 
