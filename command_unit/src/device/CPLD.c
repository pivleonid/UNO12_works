/*!  \file 	CPLD.c
*   \brief  файл функций настройки работы платы преселектора и гетеродина
*/

/*includes==========================================================================================================*/
#include "CPLD.h"
#include <stm32f4xx_hal.h>
/*defines===========================================================================================================*/
#define ADDR1 GPIO_PIN_15
#define ADDR0 GPIO_PIN_14
#define ADDR2 GPIO_PIN_13
#define STROB GPIO_PIN_12
#define data0 GPIO_PIN_0
#define data1 GPIO_PIN_1
#define data2 GPIO_PIN_2
#define data3 GPIO_PIN_3
#define data4 GPIO_PIN_4
#define data5 GPIO_PIN_5
#define data6 GPIO_PIN_6
#define data7 GPIO_PIN_7
/*types=============================================================================================================*/
typedef struct CPLD_out
{
	uint8_t data_CPLD_0_0;
	uint8_t data_CPLD_0_1;
	uint8_t data_CPLD_0_2;
	uint8_t data_CPLD_1_0;
	uint8_t data_CPLD_1_1;
	uint8_t data_CPLD_1_2;
} CPLD_out_t;
/*------------------------------------------------------------*/
enum AT_ID
{
	LNA = 1,  /*low noise amplifier -> малошум€щий усилитель*/
	LNA_1_GHz_preselector,
	Attenuator_0_65_dB,
	RF_TEST,
	state_preselector_0x05,
	/*6  состо€ние описано далее в функции*/
	Preseloctor_ON_OFF = 7,
	HighBand_LowBand,
	Ymnog_UNO1 = 0x34, /*—осто€ни€ гетеродина*/
};
/*------------------------------------------------------------*/
const  CPLD_out_t   state_preselector[53][14] = {
	[LNA][0] = { 0, 0, 0, 0, 0, 0 },
	[LNA][1] = { 0, 0, 0, 0, 0b00010000, 0 },
	[LNA][2] = { 0, 0, 0, 0, 0b00011000, 0 },

	[LNA_1_GHz_preselector][0] = { 0, 0, 0, 0, 0, 0 },
	[LNA_1_GHz_preselector][1] = { 0, 0, 0, 0, 0b00100000, 0 },

	[Attenuator_0_65_dB][0] = { 0, 0, 0, 0, 0, 0 },
	[Attenuator_0_65_dB][1] = { 0, 0, 0, 0 , 0, 0b00000001 },
	[Attenuator_0_65_dB][2] = { 0, 0, 0, 0 , 0, 0b00000010 },
	[Attenuator_0_65_dB][3] = { 0, 0, 0, 0, 0, 0b00000011 },
	[Attenuator_0_65_dB][4] = { 0, 0, 0, 0, 0, 0b00000100 },
	[Attenuator_0_65_dB][5] = { 0, 0, 0, 0, 0, 0b00000101 },
	[Attenuator_0_65_dB][6] = { 0, 0, 0, 0, 0, 0b00000110 },
	[Attenuator_0_65_dB][7] = { 0, 0, 0, 0, 0, 0b00000111 },
	[Attenuator_0_65_dB][8] = { 0, 0, 0, 0, 0, 0b00001010 },
	[Attenuator_0_65_dB][9] = { 0, 0, 0, 0, 0, 0b00001011 },
	[Attenuator_0_65_dB][10] = { 0, 0, 0, 0, 0, 0b00001100 },
	[Attenuator_0_65_dB][11] = { 0, 0, 0, 0, 0, 0b00001101 },
	[Attenuator_0_65_dB][12] = { 0, 0, 0, 0, 0, 0b00001110 },
	[Attenuator_0_65_dB][13] = { 0, 0, 0, 0, 0, 0b00001111 },

	[RF_TEST][0] = { 0, 0, 0, 0, 0, 0 },
	[RF_TEST][1] = { 0, 0, 0, 0, 0b00000010, 0 },

	[state_preselector_0x05][0] = { 0b10010010, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][1] = { 0b10110010, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][2] = { 0b10101010, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][3] = { 0b10001010, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][4] = { 0b10111110, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][5] = { 0b10111101, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][6] = { 0b10111001, 0, 0, 0, 0, 0 },
	[state_preselector_0x05][7] = { 0b01111111, 0, 0, 1, 0, 0 },
	[state_preselector_0x05][8] = { 0b10111111, 1, 0, 2, 0, 0 },
	[state_preselector_0x05][9] = { 0b01111111, 1, 0, 4, 0, 0 },
	[state_preselector_0x05][10] = { 0, 0, 0b00001000, 0, 0, 0 },
	[state_preselector_0x05][11] = { 0, 0, 0b00000100, 0, 0, 0 },
	[state_preselector_0x05][12] = { 0, 0, 0b00000010, 0, 0, 0 },
	[state_preselector_0x05][13] = { 0, 0, 0b00000001, 0, 0, 0 },

	[Preseloctor_ON_OFF][0] = { 0, 0, 0, 0, 0, 0 },
	[Preseloctor_ON_OFF][1] = { 0, 0, 0, 0, 0b01000000, 0 },

	[HighBand_LowBand][0] = { 0, 0, 0, 0, 0, 0 },
	[HighBand_LowBand][1] = { 0, 0, 0, 0, 0b10000000, 0 },

	[Ymnog_UNO1][0] = { 0b11010000, 0, 0, 0, 0, 0 },
	[Ymnog_UNO1][1] = { 0b11100110, 0, 0, 0, 0, 0 },
	[Ymnog_UNO1][2] = { 0b10101101, 1, 0, 0, 0, 0 },
	[Ymnog_UNO1][3] = { 0b10100100, 1, 0, 0, 0, 0 },

};
/*prototypes========================================================================================================*/
static void set_3_byte(uint8_t data_byte1, uint8_t data_byte2, uint8_t data_byte3, uint8_t CPLD);
static void set_byte(uint8_t data_byte, uint8_t addr_CPLD);
static void set_output_cpld(void);
static uint8_t func_0x06(uint8_t AT_ID);
/*code==============================================================================================================*/

/*=============================================================================================================*/
/*!  \brief
ћен€ем биты местами
\return uint8_t
\retval
\sa
*/
/*=============================================================================================================*/
//------------------- ќбработка 0x06 --------------------------//
// D0 1 байт (кроме 0 бита), впринципе перевод работает так, что
// срать конем на этот бит
static uint8_t func_0x06(uint8_t number)
{
	uint16_t code_out = 0;
	code_out = (number & 0x01) << 15;
	code_out |= (number & 0x02) << 13;
	code_out |= (number & 0x04) << 11;
	code_out |= (number & 0x8) << 9;
	code_out |= (number & 0x10) << 7;
	code_out |= (number & 0x20) << 5;
	code_out |= (number & 0x40) << 3;
	code_out |= (number & 0x80) << 1;
	uint8_t code_out_1 = 0;
	code_out_1 = (uint8_t)(code_out >> 8);
	return (code_out_1);
}


/*=============================================================================================================*/
/*!  \brief
”становка значений на CPLD
\return void
\retval
\sa
*/
/*=============================================================================================================*/
void cpld_write(uint8_t AT_ID, uint8_t number)
{
	if (AT_ID == 0x06) set_3_byte(0, func_0x06(number), 0, 0);
	if (AT_ID == 0x34) set_3_byte(state_preselector[AT_ID][number].data_CPLD_0_0, state_preselector[AT_ID][number].data_CPLD_0_1, state_preselector[AT_ID][number].data_CPLD_0_2, 0);

	else
	{
		set_3_byte(state_preselector[AT_ID][number].data_CPLD_0_0, state_preselector[AT_ID][number].data_CPLD_0_1, state_preselector[AT_ID][number].data_CPLD_0_2, 0);
		set_3_byte(state_preselector[AT_ID][number].data_CPLD_1_0, state_preselector[AT_ID][number].data_CPLD_1_1, state_preselector[AT_ID][number].data_CPLD_1_2, 1);
	}

}
/*------------------------------------------------------------*/
static void set_3_byte(uint8_t data_byte1, uint8_t data_byte2, uint8_t data_byte3, uint8_t CPLD)
{
	if (CPLD == 0)
	{
		set_byte(data_byte1, 1);
		set_byte(data_byte2, 2);
		set_byte(data_byte3, 3);
	}
	if (CPLD == 1)
	{
		set_byte(data_byte1, 4);
		set_byte(data_byte2, 5);
		set_byte(data_byte3, 6);
	}
	set_output_cpld();

	//return ERR!
}
/*------------------------------------------------------------*/
/*
јдреса дл€ записи:
PIN_13	  PIN_15 PIN_14
RD        CS1    CS0
adder_CPLD	 ADR2   ADR1    ADR0
(0)			 0    0    0 - не используетс€

(1)			 0    0    1 - нулевой байт первой CPLD
(2)			 0    1    0 - первый  байт первой CPLD
(3)			 0    1    1 - второй  байт первой CPLD

(4)			 1    0    0 - нулевой байт второй CPLD
(5)			 1    0    1 - первый  байт второй CPLD
(6)			 1    1    0 - второй  байт второй CPLD

(7)			 1    1    1 - прокидка состо€ний внутреннего регистра на выход дл€ обоих CPLD
*/
//data byte - данные одного байта! addr_CPLD -> перва€ CPLD 1,2,3 втора€ -> 4,5,6 
/*------------------------------------------------------------*/
static void set_byte(uint8_t data_byte, uint8_t addr_CPLD)
{
	HAL_GPIO_WritePin(GPIOD, data0 | data1 | data2 | data3 | data5 | data4 | data6 | data7 | ADDR0 | ADDR1 | ADDR2 | STROB, GPIO_PIN_RESET);
	//HAL_Delay(5);
	uint8_t numb_0 = data_byte & 0x01;
	uint8_t numb_1 = data_byte & 0x02;
	uint8_t numb_2 = data_byte & 0x04;
	uint8_t numb_3 = data_byte & 0x08;
	uint8_t numb_4 = data_byte & 0x10;
	uint8_t numb_5 = data_byte & 0x20;
	uint8_t numb_6 = data_byte & 0x40;
	uint8_t numb_7 = data_byte & 0x80;
	/*
	јлгоритм работы тернарной условной операции ?: следующий:
	(логическое выражение ? выражение 1 : выражение 2)
	1. ¬ычисл€етс€ логическое выражение.
	2. ≈сли логическое выражение истинно, то вычисл€етс€ значение выражени€ выражение 1, в противном случае (0) Ч значение выражени€ выражение 2.
	3. ¬ычисленное значение возвращаетс€.
	*/
	HAL_GPIO_WritePin(GPIOD,
		(numb_0 ? data0 : 0) |
		(numb_1 ? data1 : 0) |
		(numb_2 ? data2 : 0) |
		(numb_3 ? data3 : 0) |
		(numb_4 ? data4 : 0) |
		(numb_5 ? data5 : 0) |
		(numb_6 ? data6 : 0) |
		(numb_7 ? data7 : 0),
		GPIO_PIN_SET);
	uint8_t addr_0 = addr_CPLD & 0x01;
	uint8_t addr_1 = addr_CPLD & 0x02;
	uint8_t addr_2 = addr_CPLD & 0x04;
	HAL_GPIO_WritePin(GPIOD,
		((addr_0 ? ADDR0 : 0) |
		(addr_1 ? ADDR1 : 0) |
			(addr_2 ? ADDR2 : 0)),
		GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, STROB, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, STROB, GPIO_PIN_RESET);
}
/*------------------------------------------------------------*/
static void set_output_cpld(void)
{
	HAL_GPIO_WritePin(GPIOD, data0 | data1 | data2 | data3 | data5 | data4 | data6 | data7 | ADDR0 | ADDR1 | ADDR2 | STROB, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOD, ADDR0 | ADDR1 | ADDR2, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOD, ADDR0 | ADDR1 | ADDR2, GPIO_PIN_RESET);
}

/*=============================================================================================================*/
/*!  \brief
включение пинов дл€ работы с CPLD
\return void
\retval
\sa
*/
/*=============================================================================================================*/
void cpld_open(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	HAL_GPIO_WritePin(GPIOD,
		GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
		| GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
		| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7,
		GPIO_PIN_RESET);
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
		| GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
		| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/*=============================================================================================================*/
/*!  \brief
¬ыключение пинов дл€ работы с CPLD
\return void
\retval
\sa
*/
void cpld_close(void)
{
	//выключение портов
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15
		| GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3
		| GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}
/*------------------------------------------------------------*/
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
