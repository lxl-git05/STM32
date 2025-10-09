#include "Serial.h"
#include "usart.h"
#include "main.h"
#include "string.h"

uint8_t USART_RX_BUF[USART_RX_BUF_LEN] ;
int Serial_RxData ;
int Serial_RxFlag ;

// 串口发送一个字节
void Serial_SendByte(uint8_t Byte)
{
	HAL_UART_Transmit_IT(&huart1, &Byte, 1);
	// 检查UART是否就绪,如果不等待肯定出问题
	while (huart1.gState != HAL_UART_STATE_READY) ;
}

// 串口发送数组
void Serial_SendArray(uint8_t *Array , uint16_t Length)
{
	uint16_t i ;
	for (i = 0 ; i < Length ; i ++)
	{
		Serial_SendByte(Array[i]) ;
	}
}

// 串口发送字符串
void Serial_SendString(char *String)
{
	uint8_t i ;
	for (i = 0 ; String[i] != 0 ; i ++)
	{
		Serial_SendByte(String[i]) ;
	}
}

// 指数运算
uint32_t Serial_Pow(uint32_t X , uint32_t Y)
{
	uint32_t Result = 1 ;
	while(Y--)
	{
		Result *= X ;
	}
	return Result ;
}

// 串口发送数字串
void Serial_SendNumber(uint32_t Number , uint8_t Length)
{
	uint8_t i ;
	for(i = 0 ; i < Length ; i ++)
	{
		Serial_SendByte(Number / Serial_Pow(10 , Length - i - 1) % 10 + '0') ;
	}
}

// 串口printf重定向
int fputc(int ch , FILE *f)
{
	Serial_SendByte(ch) ;
	return ch ;
}

// 得到串口接收标志位
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)			//如果标志位为1
	{
		Serial_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}

// 得到串口接收数据
uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;			//返回接收的数据变量
}

// USART串口通信
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == USART1)
	{
		// 1.添加结束符确保字符串安全
		if (Size > 0 && Size < USART_RX_BUF_LEN) 
		{
			USART_RX_BUF[Size] = '\0';
		}

		// 2.使用字符串匹配代替固定长度检查
		if (strstr((char *)USART_RX_BUF, "send") != NULL)
		{
			HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
		}

		// 3. 发送回显（添加\r\n+正确长度+状态检查）
		const char *echo_str = "Hello!!!"; // 不换行的回显字符串
		uint16_t echo_len = strlen(echo_str); // 计算长度
	
		if (huart1.gState == HAL_UART_STATE_READY) 
		{ 
			HAL_UART_Transmit_IT(&huart1, (uint8_t *)echo_str, echo_len); // 检查UART是否就绪
		}

		// 4.重启接收前清除缓存区
		memset(USART_RX_BUF, 0, USART_RX_BUF_LEN);
	
		// 5.再次接收
		HAL_UARTEx_ReceiveToIdle_IT(&huart1, USART_RX_BUF, USART_RX_BUF_LEN);
	}
}
