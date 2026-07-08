#ifndef __MYMAIN_H
#define __MYMAIN_H

// 系统
#include "main.h"
#include <stdlib.h>
#include "string.h"
#include <stdio.h>
#include <math.h>

//#include "tim.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "stdbool.h"

// 驱动
#include "OLED.h"
#include "Key.h"
#include "Serial.h"
#include "Task.h"
#include "bsp_at24c02.h"
#include "at24c02_manager.h"

#include "Encoder.h"
#include "Timer_Counter.h"
#include "LED_Flash.h"
#include "Key_Check.h"
#include "Menu_Key.h"
#include "Param_AT24C02.h"

// 函数声明
void Mymain(void) ;

#endif
