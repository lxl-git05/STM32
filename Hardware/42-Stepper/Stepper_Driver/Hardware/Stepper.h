#ifndef __STEPPER_H
#define __STEPPER_H

#include "MySystem.h"

// 串口驱动步进电机

// 接收数据的宏定义
#define		CMD_LEN		255
extern __IO bool rxFrameFlag;
extern __IO uint8_t rxCmd[CMD_LEN];
extern __IO uint8_t rxCount;

// 串口定义
#define Stepper_huart huart3

// 1. 初始化
void Stepper_Init(void) ;
// 2. 串口RX_DMA处理,放在stm32h7xx_it.c的相应串口位置
void Stepper_Rx_DMA_Cplt(void) ;

#endif
