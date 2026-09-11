#include "PWM.h"
#include "tim.h"

void PWM_Init(void)
{
	// TIM2--PWM的初始化,选择四个通道中的通道1(注意要先在Cube中选择使能相应通道channel)
	HAL_TIM_PWM_Start(&htim2 , TIM_CHANNEL_1) ;
}

void PWM_SetCompare1(uint16_t Compare)
{
	// 设置占空比,PWM需要在0-ARR之间
	__HAL_TIM_SET_COMPARE(&htim2 , TIM_CHANNEL_1 , Compare ) ;
}

