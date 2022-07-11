#ifndef DEL_ESP01S_H
#define DEL_ESP01S_H

#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>


#include "usart.h"

#define ESP_HANDLE huart1

#define SendData(s) WRITE_REG(ESP_HANDLE.Instance->DR,s);
#define ESP_printf(fmt, ...)  USART_printf (USART2, fmt, ##__VA_ARGS__)

#define RX_BUF_MAX_LEN 1024       //最大字节数
extern struct STRUCT_USART_Fram   //数据帧结构体
{
    char Data_RX_BUF[RX_BUF_MAX_LEN];
		uint16_t FramLength;
		uint8_t FramFinishFlag;

	
}ESP_Fram_Record_Struct;

void Esp_SendString(char *str);
bool ESP_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,uint32_t time);
bool ESP_Begin_Server(void);
bool ESP_End_Server(void);
bool ESP_Client_Connect(void);
bool ESP_Message_Available(void);

void USART_printf( USART_TypeDef * USARTx, char * Data, ... );

void ESP_IRQHandler(void);

#endif
