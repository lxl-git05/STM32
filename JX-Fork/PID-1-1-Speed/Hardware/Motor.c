#include "main.h"
#include "Motor.h"
#include "PWM.h"

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	// Motor引脚已经定义了,所以没什么好Init的
	PWM_Init() ;
}

/**
  * 函    数：直流电机设置PWM
  * 参    数：PWM 要设置的PWM值，范围：-100~100（负数为反转）
  * 返 回 值：无
  */
void Motor_SetPWM(int8_t PWM)
{
	if (PWM >= 0)							//如果设置正转的PWM
	{
		
		HAL_GPIO_WritePin(Motor_AIN1_GPIO_Port , Motor_AIN1_Pin , GPIO_PIN_RESET) ;	//PB12置低电平
		HAL_GPIO_WritePin(Motor_AIN2_GPIO_Port , Motor_AIN2_Pin , GPIO_PIN_SET) ;		//PB13置高电平
		PWM_SetCompare1(PWM);				//设置PWM占空比
	}
	else									//否则，即设置反转的速度值
	{
		HAL_GPIO_WritePin(Motor_AIN1_GPIO_Port , Motor_AIN1_Pin , GPIO_PIN_SET) ;	  //PB12置高电平
		HAL_GPIO_WritePin(Motor_AIN2_GPIO_Port , Motor_AIN2_Pin , GPIO_PIN_RESET) ;	//PB13置低电平
		PWM_SetCompare1(-PWM);				//设置PWM占空比
	}
}
