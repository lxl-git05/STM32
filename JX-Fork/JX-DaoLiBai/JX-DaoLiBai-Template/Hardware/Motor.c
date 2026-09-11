#include "Motor.h"
#include "PWM.h"

void Motor_Init(void)
{
	PWM_Init();
}

void Motor_SetPWM(int8_t PWM)
{
	if (PWM >= 0)
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_RESET) ;
		HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_SET) ;
		PWM_SetCompare1(PWM);
	}
	else
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_SET) ;
		HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_RESET) ;
		PWM_SetCompare1(-PWM);
	}
}