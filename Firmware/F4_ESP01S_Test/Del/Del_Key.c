#include "Del_Key.h"

void Button_Init(BtnHandle * handle)
{
	handle->btn_20ms_count=0;
	handle->btn_event = BUTTON_EVENT_NONE;
	handle->btn_state = BUTTON_OFF;
}

void Button_Scan_20ms(BtnHandle handle_btn[],uint8_t btn_num)
{
	uint8_t i;
	for(i = 0;i<btn_num;i++)
	{
		handle_btn[i].btn_state = (HAL_GPIO_ReadPin(handle_btn[i].btn_port, handle_btn[i].btn_pin) == BUTTON_PRESSDOWN) ? BUTTON_ON : BUTTON_OFF;
		switch(handle_btn[i].btn_event)
		{
			case BUTTON_EVENT_NONE:
				if(handle_btn[i].btn_state)
				{
					handle_btn[i].btn_event = BUTTON_EVENT_SHAKE;
				}
				break;
			case BUTTON_EVENT_SHAKE:
				if(handle_btn[i].btn_state)
				{
					handle_btn[i].btn_event = BUTTON_EVENT_SHORTPRESS;
					handle_btn[i].btn_20ms_count++;
				}
				else
				{
					handle_btn[i].btn_event = BUTTON_EVENT_NONE;
					handle_btn[i].btn_20ms_count=0;
				}
				break;
			case BUTTON_EVENT_SHORTPRESS:
				if(handle_btn[i].btn_state){
					if(handle_btn[i].btn_20ms_count>=BUTTON_LONGPRESS_COUNT)
					{
						handle_btn[i].btn_event = BUTTON_EVENT_LONGPRESS;
						handle_btn[i].btn_long_cb();
						handle_btn[i].btn_20ms_count = 0;
					}
					else
					{
						handle_btn[i].btn_20ms_count++;
					}
				}
				else
				{
					handle_btn[i].btn_short_cb();
					handle_btn[i].btn_event = BUTTON_EVENT_NONE;
					handle_btn[i].btn_20ms_count = 0;
				}
				break;
			case BUTTON_EVENT_LONGPRESS:
				if(handle_btn[i].btn_state == BUTTON_OFF)
				{
					handle_btn[i].btn_event = BUTTON_EVENT_NONE;
					handle_btn[i].btn_20ms_count = 0;
				}
				break;
		}
	}
}

