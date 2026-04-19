#include "Mode_3.h"
#include "AllHeader.h"

float check ;

void Mode_3_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0, 0, OLED_6X8, "=====Mode_3=====") ;
}

void Mode_3_Loop(void)
{
	// 本loop函数建议只执行一个check任务,防止未知Bug
    
    // 2. 测试电机
    
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
