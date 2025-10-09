#ifndef __SERIAL_H
#define __SERIAL_H

#include "stdio.h"
#include "main.h"

// 定义接受长度
#define USART_RX_BUF_LEN 25 

void Serial_Rx_init(void);

void Serial_SendByte(uint8_t Byte);

void Serial_SendArray(uint8_t *Array , uint16_t Length) ;

void Serial_SendString(char *String) ;

void Serial_SendNumber(uint32_t Number , uint8_t Length) ;

uint8_t Serial_GetRxFlag(void) ;

uint8_t Serial_GetRxData(void) ;
	
#endif
