#ifndef DEL_DEBUG_H
#define DEL_DEBUG_H

#include "stm32f4xx_hal.h"
#include "usart.h"

#define DEBUG_HANDLE	huart2

void Debug_SendString(char * str);

#endif
