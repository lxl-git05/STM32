#ifndef __SERIAL_H
#define __SERIAL_H

#include "main.h"
#include <stdbool.h>

#define Serial_huart huart1
#define Serial_USART USART1

// DMA接收数组长度
#define RX_Serial_LEN 50

// 数据通过DMA发送
void Serial_SendData_DMA(uint8_t *pData, uint16_t Size) ;

// HEX: 判断是否更新数据
uint8_t Serial_GetNewPackageFlag_HEX(void) ;
// HEX: 得到错误原因
int Serial_GetError_HEX(void) ;

// 文本:判断是否更新数据
uint8_t Serial_GetNewPackageFlag_ABC(void) ;
// 文本:得到错误原因
int Serial_GetError_ABC(void) ;
// 文本1:数据包指令改变浮点数据大小
bool Serial_SetFloatData( char *KeyWord , char *cmd , float *Data) ;
// 文本2:数据包指令改变整型数据大小
bool Serial_SetIntData( char *KeyWord , char *cmd , int *Data) ;

#endif
