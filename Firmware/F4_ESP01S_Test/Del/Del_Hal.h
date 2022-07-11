#ifndef DEL_HAL_H
#define DEL_HAL_H

#include "stm32f4xx_hal.h"

#include "tim.h"
#include "Del_Key.h"
#include "Del_ESP01S.h"
#include "Del_Debug.h"

#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13

#define KEY_PORT GPIOA
#define KEY_PIN GPIO_PIN_0
#define BUTTON_NUM 1

void Del_Init(void);
void Del_Update(void);

void key_shortcb(void);
void key_longcb(void);

#endif
