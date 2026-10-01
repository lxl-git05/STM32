#ifndef USER_ISR_H
#define USER_ISR_H

#include "stm32f1xx_hal.h"

void ISR_TIM_Handler(TIM_HandleTypeDef *htim);

#endif /* USER_ISR_H */
