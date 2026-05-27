#include "Mode_4.h"
#include "AllHeader.h"

typedef enum
{
	Robot_Init ,					// 电机在顶端, 夹子张开,晾衣杆在另一侧(Back),衣架闭合
	Robot_Down ,					// 电机向下够衣服,两次
	Robot_Claw_Close , 		// 夹衣服
	Robot_Arm_Come,				// 晾衣杆到同侧
	Robot_Up ,						// 电机上升,
	Robot_Hanger_Open ,		// 衣架张开
	Robot_Claw_Open  , 		// 夹爪松开(张开)
	Robot_OK,							// 电机重新回到顶
}Robot_Status_Typedef ;

Robot_Status_Typedef Robot_Status = Robot_Init;
bool Robot_Wait_Cmd = false ;

// 定义各项参数
void Mode_4_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0,5,OLED_8X16,"================") ;
	 OLED_ShowChinese(0,25,"欢迎使用有衣有靠") ;
	 OLED_Printf(0,45,OLED_8X16,"================") ;
}

// 预备控制
void Mode_4_Loop(void)
{
	Robot_Wait_Cmd = true ;
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Robot_Wait_Cmd = true ;
	}
	if (Key_Check(KEY_2 , KEY_SINGLE))
	{
		Robot_Status = Robot_Init ;
		Motor_B.Motor_Encoder->total_cnt = 0 ;
	}
	if (Serial_GetNewPackageFlag_ABC(&Serial2))
	{
		 // 1. 晾衣架开始 晾衣服
		 if (Serial_Check_Str(&Serial2 , "Hanger_Up"))
		 {
				Robot_Wait_Cmd = true	;
		 }
	}
	// 1. 初始化
	if (Robot_Status == Robot_Init)
	{
		// 电机在顶端, 夹子张开,晾衣杆在另一侧(Back),衣架闭合
		// 1. 电机在顶端
		Motor_SetAngle(&Motor_B , 0) ;
		// 2. 夹子张开
		Servo_Claw_Open() ;
		// 3. 晾衣杆在另一侧(Back)
		Servo_Arm_Back();
		// 4. 衣架闭合
		Servo_Hanger_Close() ;
		// 进入下一个状态
		HAL_Delay(5000) ;
		Robot_Status = Robot_Down ;
	}
	else if (Robot_Status == Robot_Down && Robot_Wait_Cmd == true)
	{
		// 1. 电机向下够衣服
		Motor_SetAngle(&Motor_B , 400) ;	// 待修改
		// 进入下一个状态
		// 判断是否下降到400
		if (Motor_Is_Angle(&Motor_B , 400 , 10))
		{
			Robot_Status = Robot_Claw_Close ;
			Robot_Wait_Cmd = false ;	// DISABLE
		}
	}
	else if (Robot_Status == Robot_Claw_Close)
	{
		// 1. 夹衣服
		Servo_Claw_Close() ;
		// 进入下一个状态
		Robot_Status = Robot_Arm_Come ;
	}
	else if (Robot_Status == Robot_Arm_Come)
	{
		// 1. 晾衣杆到同侧
		Servo_Arm_Come() ;
		// 进入下一个状态
		Robot_Status = Robot_Up ;	
	}
	else if (Robot_Status == Robot_Up)
	{
		// 1. 电机上升,使得衣架能够穿过衣服
		Motor_SetAngle(&Motor_B , 135) ;	// 待修改
		// 进入下一个状态 
		if (Motor_Is_Angle(&Motor_B , 135, 10))
		{
			Robot_Status = Robot_Hanger_Open ;
		}
	}
	else if (Robot_Status == Robot_Hanger_Open)
	{
		// 1. 衣架张开
		int goal = Servo_Hanger_Open() ;
		// 进入下一个状态 
		HAL_Delay(700) ;
		Robot_Status = Robot_Claw_Open ;
	}
	else if (Robot_Status == Robot_Claw_Open)
	{
		// 1. 夹爪松开,放开衣服
		Servo_Claw_Open() ;
		// 进入下一个状态 
		Robot_Status = Robot_OK ;
	}
	else if (Robot_Status == Robot_OK)
	{
		// 1. 电机回到顶部
		Motor_SetAngle(&Motor_B , 0) ; 
		// 2. 夹子张开
		// 3. 晾衣杆在另一侧(Back)
		if (Motor_Is_Angle(&Motor_B , 0, 35))
		{
			Servo_Arm_Back();
		}
		// 4. 衣架不闭合
	}
//	OLED_ClearArea(0,20,128,10) ;
//	OLED_Printf(0,20,OLED_6X8 , "Angle= %f" , Motor_B.PID_Angle.realPoint_Now) ;
}

void Mode_4_Tick(void)
{
	
}

void Mode_4_Exit(void)
{
  
}
