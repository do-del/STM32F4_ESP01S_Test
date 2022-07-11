#include "Del_ESP01S.h"

struct STRUCT_USART_Fram ESP_Fram_Record_Struct = { 0 };  //定义了一个数据帧结构体

static char *itoa( int value, char *string, int radix );

void ESP_IRQHandler(void)
{
	uint8_t ch;
	if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE) != RESET)
	{
		ch = (uint8_t)READ_REG(huart1.Instance->DR);
		WRITE_REG(huart2.Instance->DR,ch);
		
		if(ESP_Fram_Record_Struct.FramLength < (RX_BUF_MAX_LEN - 1)) 
		{
			//留最后一位做结束位
			ESP_Fram_Record_Struct.Data_RX_BUF[ESP_Fram_Record_Struct.FramLength] = ch;
			ESP_Fram_Record_Struct.FramLength++;
    }
	}
	
}

void Esp_SendString(char *str)
{
	unsigned int k = 0;
	do
	{
		HAL_UART_Transmit(&ESP_HANDLE,(uint8_t *)(str+k),1,1000);
		k++;
	}while(*(str+k)!='\0');
}

bool ESP_Send_AT_Cmd(char *cmd,char *ack1,char *ack2,uint32_t time)
{
	ESP_Fram_Record_Struct.FramLength = 0; //重新接收新的数据包
	ESP_printf("%s\r\n", cmd);
	if(ack1==0&&ack2==0)     //不需要接收数据
  {
		return true;
	}
	HAL_Delay(time);   //延时
	HAL_Delay(1000);
	ESP_Fram_Record_Struct.Data_RX_BUF[ESP_Fram_Record_Struct.FramLength] = '\0';
	
	//printf("%s",ESP_Fram_Record_Struct.Data_RX_BUF);
	if(ack1!=0&&ack2!=0)
	{
		return ((bool) strstr(ESP_Fram_Record_Struct .Data_RX_BUF, ack1) || (bool)strstr(ESP_Fram_Record_Struct .Data_RX_BUF, ack2));
	}
	else if( ack1 != 0 )  //strstr(s1,s2);检测s2是否为s1的一部分，是返回该位置，否则返回false，它强制转换为bool类型了
		return ((bool)strstr(ESP_Fram_Record_Struct .Data_RX_BUF, ack1));

	else
		return ((bool)strstr(ESP_Fram_Record_Struct .Data_RX_BUF, ack2));
}

bool ESP_Begin_Server(void)
{
	if(!(ESP_Send_AT_Cmd("AT","OK",NULL,2000))) return false;
	if(!(ESP_Send_AT_Cmd("AT+CIPMODE=0","OK",NULL,500))) return false;
	if(!ESP_Send_AT_Cmd("AT+CIPMUX=1","OK",NULL,500)) return false;
	if(!ESP_Send_AT_Cmd("AT+CIPSERVER=1,8080","OK",NULL,500)) return false;
	if(!ESP_Send_AT_Cmd("AT+CIPSTO=1800","OK",NULL,500)) return false;
	printf("Waiting for client\r\n");
	ESP_Fram_Record_Struct.FramLength = 0; //重新接收新的数据包
	return true;
}

bool ESP_End_Server(void)
{
	if(!ESP_Send_AT_Cmd("AT+CIPSERVER=0","OK",NULL,500)) return false;
	return true;
}

bool ESP_Client_Connect(void)
{
	char client_id;
	if(!ESP_Fram_Record_Struct.FramLength)
	{
		//Update UI
		//printf("waiting\r\n");
		HAL_Delay(500);
		return false;
	}
	HAL_Delay(1000);
	ESP_Fram_Record_Struct.Data_RX_BUF[ESP_Fram_Record_Struct.FramLength] = '\0';
	if(strstr(ESP_Fram_Record_Struct .Data_RX_BUF, "CONNECT"))
	{
		printf("Client Connected,id:");
		client_id = ESP_Fram_Record_Struct .Data_RX_BUF[0];
		printf("%c\r\n",client_id);
		ESP_Fram_Record_Struct.FramLength = 0; //重新接收新的数据包
		return true;
	}
	ESP_Fram_Record_Struct.FramLength = 0; //重新接收新的数据包
	return false;
}

bool ESP_Message_Available(void)
{
	int msg;
	int a,b;
	if(!ESP_Fram_Record_Struct.FramLength)
	{
		//Update UI
		//printf("waiting\r\n");
		HAL_Delay(500);
		return false;
	}
	HAL_Delay(500);
	ESP_Fram_Record_Struct.Data_RX_BUF[ESP_Fram_Record_Struct.FramLength] = '\0';
	//printf("%s",ESP_Fram_Record_Struct.Data_RX_BUF);
	sscanf(ESP_Fram_Record_Struct.Data_RX_BUF,"\r\n+IPD,%d,%d:%d",&a,&b,&msg);
	printf("\r\n%d",msg);
	ESP_Fram_Record_Struct.FramLength = 0; //重新接收新的数据包
	return true;
}


static char *itoa( int value, char *string, int radix )
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;

    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

void USART_printf ( USART_TypeDef * USARTx, char * Data, ... )
{
    const char *s;
    int d;   
    char buf[16];


    va_list ap;
    va_start(ap, Data);

    while ( * Data != 0 )     // 判断数据是否到达结束符
    {                                         
        if ( * Data == 0x5c )  //'\'
        {                                     
            switch ( *++Data )
            {
                case 'r':                                     //回车符
                SendData(0x0d);
                Data ++;
                break;

                case 'n':                                     //换行符
                SendData(0x0a); 
                Data ++;
                break;

                default:
                Data ++;
                break;
            }            
        }

        else if ( * Data == '%')
        {                                     
            switch ( *++Data )
            {               
                case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++) 
                {
                    SendData(*s);
									
                    while( __HAL_UART_GET_FLAG(&ESP_HANDLE,UART_FLAG_TXE) == RESET );
                }
                Data++;
                break;

                case 'd':           
                    //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++) 
                {
                    SendData(*s);
                    while( __HAL_UART_GET_FLAG(&ESP_HANDLE,UART_FLAG_TXE) == RESET );
                }
                     Data++;
                     break;
                default:
                     Data++;
                     break;
            }        
        }
        else SendData(*Data++);
        while ( __HAL_UART_GET_FLAG(&ESP_HANDLE,UART_FLAG_TXE) == RESET );

    }
}
