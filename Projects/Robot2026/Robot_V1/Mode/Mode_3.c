#include "Mode_3.h"
#include "AllHeader.h"

#define Check_Num 3
int Check_Item = 0;

// 测试全局变量
float check ;

// 测试函数声明
void Check_Serial(Serial_Typedef* pSerial);
void Check_PWM(void) ;  // 需要初始化
void Check_Encoder(void) ;  // 需要初始化

void Mode_3_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0, 0, OLED_6X8, "=====Mode_Test=====") ;
   // 测试涉及的初始化
   MyPWM_Init(&MyPWM_Servo1) ;
   MyEncoder_Init(&Motor_A_Encoder) ;
}

void Mode_3_Loop(void)
{
	// 长按切换测试项目:本loop函数只执行一个check任务,防止未知Bug
    if (Key_Check(KEY_0 , KEY_LONG))
    {
        Check_Item ++ ;
        if (Check_Item > Check_Num)
        {
            Check_Item = 0 ;
        }
    }
    switch (Check_Item)
    {
        case 0:
            OLED_Printf(0, 10, OLED_6X8, "check1") ;
            Check_Serial(&Serial1) ;
            break;
        case 1:
        	OLED_Printf(0, 10, OLED_6X8, "check2") ;
            Check_PWM() ;
            break;
        case 2:
            OLED_Printf(0, 10, OLED_6X8, "check3") ;
            Check_Encoder() ;
            break;
        default:
            break;
    }
}

void Mode_3_Exit(void)
{
    OLED_Clear() ;
}

void Mode_3_Tick(void)
{
	
}

// 1. 测试串口功能
void Check_Serial(Serial_Typedef* pSerial)
{
    if (Serial_GetNewPackageFlag_ABC(pSerial))
    {
        Serial_SetFloatData(pSerial, "Kp", "Kp=%f", &check) ;
        Serial_printf(pSerial , "%f\n", &check) ;
    }
    OLED_Printf(0, 40, OLED_6X8, "%.2f" , check) ;  // 测试OLED
}

// 2. 测试PWM功能, 记得先初始化哦
void Check_PWM(void)
{
    static int PWM_Servo_Check = 50;    // 50-250
    if (Key_Check(KEY_0, KEY_LONG))
    {
        PWM_Servo_Check += 50 ;
        if (PWM_Servo_Check > 250)
        {
            PWM_Servo_Check = 50 ;
        }
    }
    MyPWM_SetCompare(&MyPWM_Servo1, PWM_Servo_Check) ;
}

// 3. 测试编码器功能, 记得先初始化哦
void Check_Encoder(void)
{
    OLED_Printf(0, 40, OLED_6X8, "%d" , MyEncoder_Get_CNT(&Motor_A_Encoder)) ;
}
