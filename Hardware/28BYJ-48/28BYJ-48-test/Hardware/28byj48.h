#ifndef __28BYJ48_H
#define __28BYJ48_H

#include "main.h"
// 初始化驱动BYJ的定时器中断
void BYJ_Delay_Init(void);
// 8拍驱动
void BYJ_Half_Drive(uint8_t step) ;

#endif
