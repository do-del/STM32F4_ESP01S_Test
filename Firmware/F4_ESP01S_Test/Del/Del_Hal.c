#include "Del_Hal.h"

BtnHandle handle_btn_group[BUTTON_NUM];

static uint8_t short_flag = 0;
static uint8_t long_flag = 0;
static uint8_t client_flag = 0;
static uint8_t client_available = 0;

void Del_Init(void)
{
	handle_btn_group[0].btn_port = KEY_PORT;
	handle_btn_group[0].btn_pin = KEY_PIN;
	handle_btn_group[0].btn_short_cb = key_shortcb;
	handle_btn_group[0].btn_long_cb = key_longcb;
	Button_Init(&handle_btn_group[0]);
	
	HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_SET);
	
	HAL_TIM_Base_Start_IT(&htim2);
	
	printf("ESP01S Test\r\n");
	
}

void Del_Update(void)
{
	if(short_flag)
	{
		short_flag = 0;
		ESP_Begin_Server();
		client_flag = 1;
	}
	if(long_flag)
	{
		long_flag = 0;
		ESP_End_Server();
		client_flag = 0;
		client_available = 0;
	}
	
	if(client_flag && (!client_available))
	{
		if(ESP_Client_Connect())
		{
			client_available = 1;
		}
	}
	if(client_available)
	{
		ESP_Message_Available();
	}
	
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM2)
	{
		Button_Scan_20ms(handle_btn_group,BUTTON_NUM);
	}
}

void key_shortcb(void)
{
	HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_RESET);
	if(!short_flag) short_flag = 1;
}

void key_longcb(void)
{
	HAL_GPIO_WritePin(LED_PORT,LED_PIN,GPIO_PIN_SET);
	if(!long_flag) long_flag = 1;
}
