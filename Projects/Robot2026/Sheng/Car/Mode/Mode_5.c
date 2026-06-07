#include "Mode_5.h"
#include "AllHeader.h"

extern int Base_Speed ;

// setup
void Mode_5_Setup(void)
{
	 OLED_Printf(0,5,OLED_8X16,"================") ;
	 OLED_ShowChinese(0,25,"欢迎使用有衣有靠") ;
	 OLED_Printf(0,45,OLED_8X16,"================") ;
}

// loop
void Mode_5_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"====Mode_Main====") ;
	// 测试
	if (Key_Check(KEY_1,KEY_SINGLE))
	{
		HAL_Delay(5000) ;
		next_Status = Car_Turn_F ;
	}
	if (Key_Check(KEY_2,KEY_SINGLE))
	{
		HAL_Delay(5000) ;
		next_Status = Car_Turn_F ;
	}
	// OLED展示
	OLED_Printf(0,20,OLED_6X8 , "Pos: A: %.4f", Motor_A.PID_Pos.realPoint_Now) ;
	OLED_Printf(0,30,OLED_6X8 , "Pos: B: %.4f", Motor_B.PID_Pos.realPoint_Now) ;
	OLED_Printf(0,40,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
}

// 中断控制器
void Mode_5_Tick(void)
{
	Car_Control_Change() ;
	Car_Control() ;
}

void Mode_5_Exit(void)
{
  
}
