#ifndef __SERIAL_H
#define __SERIAL_H

#include "stdio.h"
#include "main.h"

// 定义IDE接受长度
#define USART_RX_BUF_LEN 25 
// 定义数据包整体长度
#define Data_Serial 15	
// 定义溢出长度
#define OutLen 20

void Serial_Rx_init(void);

void Serial_SendByte(uint8_t Byte);

void Serial_SendArray(uint8_t *Array , uint16_t Length) ;

void Serial_SendString(char *String) ;

void Serial_SendNumber(uint32_t Number , uint8_t Length) ;

uint8_t Serial_GetRxFlag(void) ;	// ***重要,得到有效数据接收成功标志位***

uint8_t Serial_GetRxData(void) ;	// 得到单个字节数据
	
int Serial_GetError(void) ;
#endif
