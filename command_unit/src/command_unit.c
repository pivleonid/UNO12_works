#include "fsm_protocol_cdc.h"
#include "cmd_proc.h"
#include "hrdwr_func.h"
#include "device/dac.h"
#include "device/InitializationUNO_v1.h"
#include "device/CPLD.h"

#ifdef __cplusplus
extern "C"
{
#endif
	#include <usbd_core.h>
	#include <usbd_cdc.h>
	#include "usbd_cdc_if.h"
	#include <usbd_desc.h>

	USBD_HandleTypeDef USBD_Device;
	void SysTick_Handler(void);
	void OTG_FS_IRQHandler(void);
	void OTG_HS_IRQHandler(void);
	extern PCD_HandleTypeDef hpcd;
	
	int VCP_read(void *pBuffer, int size);
	int VCP_write(const void *pBuffer, int size);
	extern char g_VCPInitialized;
	
#ifdef __cplusplus
}
#endif

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	    /**Configure the main internal regulator output voltage 
	    */
	__HAL_RCC_PWR_CLK_ENABLE();

	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	    /**Initializes the CPU, AHB and APB busses clocks 
	    */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 288;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 6;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		return;
	}

	    /**Initializes the CPU, AHB and APB busses clocks 
	    */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
	                            | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		return;
	}

	    /**Configure the Systick interrupt time 
	    */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

	    /**Configure the Systick 
	    */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	  /* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd);
}
#elif defined(USE_USB_HS)
void OTG_HS_IRQHandler(void)
{
	HAL_PCD_IRQHandler(&hpcd);
}
#else
#error USB peripheral type not defined
#endif

/*
	This is a basic USB CDC device sample based on the ST USB library.
	To test it out:
		1. Install the ST VCP drivers (http://www.st.com/web/en/catalog/tools/PF257938)
		2. Connect to the virtual COM port using SmarTTY or any other terminal program
		3. Type some characters and observe the output
	Read more about the sample: http://visualgdb.com/tutorials/arm/stm32/usb/
*/

static struct pt cmd_pt;


/*=============================================================================================================*/
/*!  \brief ������������� ���������� � ��������� �����

     \return -
*/
/*=============================================================================================================*/
void periph_open(void)
{
	
	HAL_Init();
	
	SystemClock_Config();
	
	USBD_Init(&USBD_Device, &VCP_Desc, 0);
	USBD_RegisterClass(&USBD_Device, &USBD_CDC);
	USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_command_unit_fops);
	USBD_Start(&USBD_Device);
	
	// i2c_open()
	
	/* TODO ���������� ��� �������� ����������� ������ */
		/* ���� eeprom							*/
		/* ���� �� ������� ���� ����			*/
		/* flash_open() */
	
	/* ��������� ��������� ������ �	����� �������, ������� ������, ect		*/
	
	/* ��������� ������� � ������������� ����������� */	
	/* ���� ��� ���������� ������� �� ���������      */
	;
	
	/* TODO ���� ��� ���� �����, ����� ��������� ��� ��������� �������� � ��������� */
	;
	if (get_cu_type() == CU_TYPE_HETERODIN){

		(void)uno_open(0);

		(void)uno_open(1);
	}
		
	/* �������������� ��������� � ���������� � ����� �������� �� */	
	(void)dac_open(get_cu_type());
	
	/* cpld open  */
	cpld_open();
	
	/* if (het) uno_open */
	;
	/* slave_spi_open */
	;
	
}


int main(void)
{
	periph_open();
    for ( ;; ) 
    {
		
        WORK_OUT_FSM_TABLE( enum CDCRECV_STATE, get_curst_cdc, get_cursig_cdc, cdc_recv_table )
        WORK_OUT_FSM_TABLE( enum CDCSEND_STATE, get_curst_sendcdc, get_cursig_sendcdc, cdc_send_table )

    	PT_SCHEDULE( pt_cmd_proc( &cmd_pt ) );	
    }


//	char byte;
//
//	for (;;)
//	{
//		if (VCP_read(&byte, 1) != 1)
//			continue;
//		VCP_write("\r\nYou typed ", 12);
//		VCP_write(&byte, 1);
//		VCP_write("\r\n", 2);
//	}

}