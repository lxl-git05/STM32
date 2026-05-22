#include "MyPWM.h"

// 0. PWM定义
MyPWM_Typedef MyPWM_Servo1      = {&htim1 , TIM_CHANNEL_1 , 2000};
MyPWM_Typedef MyPWM_Servo2      = {&htim1 , TIM_CHANNEL_2 , 2000};
MyPWM_Typedef MyPWM_Servo3      = {&htim1 , TIM_CHANNEL_3 , 2000};
MyPWM_Typedef MyPWM_Servo4      = {&htim1 , TIM_CHANNEL_4 , 2000};
MyPWM_Typedef MyPWM_Motor_A_IN1 = {&htim4 , TIM_CHANNEL_1 , 1000};
MyPWM_Typedef MyPWM_Motor_B_IN1 = {&htim4 , TIM_CHANNEL_2 , 1000};

// 1. PWM初始化
void MyPWM_Init(MyPWM_Typedef* MyPWM)
{
    HAL_TIM_PWM_Start(MyPWM->htimx , MyPWM->Channel) ;
}

// 2. 设置PWM值
void MyPWM_SetCompare(MyPWM_Typedef* MyPWM, int Compare)
{
	if (Compare > MyPWM->PWM_MAX)
    {
        __HAL_TIM_SET_COMPARE(MyPWM->htimx, MyPWM->Channel, MyPWM->PWM_MAX);
    }
    else if (Compare < 0)
    {
        __HAL_TIM_SET_COMPARE(MyPWM->htimx, MyPWM->Channel, 0);
    }
    else
    {
        __HAL_TIM_SET_COMPARE(MyPWM->htimx, MyPWM->Channel, Compare);

    }
}
