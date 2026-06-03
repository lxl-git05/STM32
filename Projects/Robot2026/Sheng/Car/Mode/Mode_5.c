#include "Mode_5.h"
#include "AllHeader.h"

extern int Base_Speed ;
extern int Forward_Distance1 ;
extern int Forward_Distance2 ;

// setup
void Mode_5_Setup(void)
{
   OLED_Clear() ;
}

// loop
void Mode_5_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"====Mode_Main====") ;
	// ²âÊÔ
	if (Key_Check(KEY_1,KEY_SINGLE))
	{
		next_Status = Car_Turn_F ;
	}
	if (Key_Check(KEY_2, KEY_SINGLE))// Ë«»÷
	{   
			Forward_Distance1 ++ ;
	}
	if (Key_Check(KEY_2, KEY_DOUBLE))// Ë«»÷
	{   
			Forward_Distance1 -- ;
	}
	// OLEDÕ¹Ê¾
	OLED_Printf(0,20,OLED_6X8 , "Pos: A: %.4f", Motor_A.PID_Pos.realPoint_Now) ;
	OLED_Printf(0,30,OLED_6X8 , "Pos: B: %.4f", Motor_B.PID_Pos.realPoint_Now) ;
	OLED_Printf(0,40,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
}

// ÖÐ¶Ï¿ØÖÆÆ÷
void Mode_5_Tick(void)
{
	Car_Control_Change() ;
	Car_Control() ;
}

void Mode_5_Exit(void)
{
  
}
