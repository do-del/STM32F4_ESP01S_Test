#ifndef DEL_KEY_H
#define DEL_KEY_H

#include "stm32f4xx_hal.h"

//若按键按下为低电平，弹起为高电平，设置BUTTON_PRESSDOWN为0，BUTTON_PRESSUP为1（默认）
//若按键按下为高电平，弹起为低电平，设置BUTTON_PRESSDOWN为1，BUTTON_PRESSUP为0
#define BUTTON_PRESSDOWN 	0
#define BUTTON_PRESSUP		1 

// @	Button State
#define BUTTON_ON 	1
#define BUTTON_OFF 	0

// @	Button Event
// 无,抖动，短按，长按
#define BUTTON_EVENT_NONE				0
#define BUTTON_EVENT_SHAKE 			1
#define BUTTON_EVENT_SHORTPRESS	2
#define BUTTON_EVENT_LONGPRESS	3

#define BUTTON_LONGPRESS_TIME (1000)
#define BUTTON_LONGPRESS_COUNT (50)	//1000ms/20ms

typedef void (*ButtonCallback)(void);

typedef struct {
	GPIO_TypeDef * btn_port; 			//按键端口
	uint16_t btn_pin;							//按键引脚
	uint8_t btn_state;						//按键状态，BUTTON_ON/BUTTON_OFF
	uint8_t btn_event;						//按键事件
	uint8_t btn_20ms_count;				//按键20ms计数器，每20ms更新一次
	ButtonCallback btn_short_cb;	//按键短按回调函数
	ButtonCallback btn_long_cb;		//按键长按回调函数
}BtnHandle;											//按键句柄结构体，由外部创建相应变量数组，需要定义按键的端口、引脚以及两个回调函数

/*
 * 按键初始化函数，需要外部创建BtnHandle变量
 * 范例：
	BtnHandle handle_btn_group[BUTTON_NUM];

	handle_btn_group[0].btn_port = BUTTON1_GPIO_PORT;
	handle_btn_group[0].btn_pin = BUTTON1_GPIO_PIN;
	handle_btn_group[0].btn_short_cb = Del_Button1_Short_Callback;
	handle_btn_group[0].btn_long_cb = Del_Button1_Long_Callback;
	Button_Init(&handle_btn_group[0]);
	
	handle_btn_group[1].btn_port = BUTTON2_GPIO_PORT;
	handle_btn_group[1].btn_pin = BUTTON2_GPIO_PIN;
	handle_btn_group[1].btn_short_cb = Del_Button2_Short_Callback;
	handle_btn_group[1].btn_long_cb = Del_Button2_Long_Callback;
	Button_Init(&handle_btn_group[1]);
*/
void Button_Init(BtnHandle * handle);	

/*
* 按键扫描函数，需要每20ms调用一次，可放入周期为20ms的定时器中断函数中
*/
void Button_Scan_20ms(BtnHandle handle_btn[],uint8_t btn_num);

#endif
