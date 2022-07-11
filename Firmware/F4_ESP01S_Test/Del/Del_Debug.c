#include "Del_Debug.h"

void Debug_SendString(char * str)
{
	unsigned int k = 0;
	do
	{
		HAL_UART_Transmit(&DEBUG_HANDLE,(uint8_t *)(str+k),1,1000);
		k++;
	}while(*(str+k)!='\0');
}
