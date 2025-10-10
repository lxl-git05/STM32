#ifndef __SERIAL_H
#define __SERIAL_H

#include "main.h"

#define Serial_huart huart1
#define Serial_USART USART1

// DMA接收数组长度
#define RX_Serial_LEN 50

// 数据通过DMA发送
void Serial_SendData_DMA(uint8_t *pData, uint16_t Size) ;
// 判断是否更新数据
uint8_t Serial_GetNewPackageFlag(void) ;

#endif
