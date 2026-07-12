#include "Mode_3.h"
#include "AllHeader.h"
#include "Stepper_PWM.h"

// =================== Mode_3 函数 ===================

void Mode_3_Setup(void)
{
    OLED_Clear();
		// 这里Stepper2暂时为1.8 / 180 = 0.01°
		Stepper_PWM_Init(&Stepper_PWM_2 , &MyPWM_Stepper2 , &MyGPIO_Stepper2 , 0.01f , STEPPER_DIR_P) ;
}

void Mode_3_Loop(void)
{
    // ========== 按键控制 ==========
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Stepper_PWM_Speed_Set(&Stepper_PWM_2 , 20) ;
	}
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		Stepper_PWM_Stop(&Stepper_PWM_2) ;
	}
}

void Mode_3_Exit(void)
{
    OLED_Clear();
}

void Mode_3_Tick(void)
{
	
}
