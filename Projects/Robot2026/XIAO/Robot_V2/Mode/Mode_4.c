#include "Mode_4.h"
#include "AllHeader.h"

int Robot_Wait_Cmd ;	// 0:初始不动 1:开始第1轮 2:第1轮结束 3:第2轮开始 4:第2轮结束

typedef enum
{
	Robot_Init ,					// 丝杆在顶端, 夹子张开,传送带位置与衣架对应,衣架闭合
	Robot_Down ,					// 电机向下够衣服
	Robot_Claw_Close , 		// 夹衣服
	Robot_Up ,						// 电机上升,
	Robot_Hanger_Open ,		// 衣架张开
	Robot_Claw_Open  , 		// 夹爪松开(张开)
	Robot_Up_2,						// 电机重新上升到顶点
	Robot_SiGan_Next,			// 丝杆移动，整个过程完成一轮
	Robot_OK,							// 第1轮完成
}Robot_Status_Typedef ;

Robot_Status_Typedef Robot_Status = Robot_Init;
bool isSecond = false ;

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
	OLED_Printf(0, 0, OLED_6X8, "=====Mode_4=====") ;
	// 第1轮晾衣服开始
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		Robot_Wait_Cmd = 1 ;
	}
	if (Serial_GetNewPackageFlag_ABC(&Serial2))
	{
		 if (Serial_Check_Str(&Serial2 , "Hanger_Up"))
		 {
				Robot_Wait_Cmd = 1	;
		 }
	}
	// 控制:
	// 1. 初始化
	if (Robot_Status == Robot_Init)
	{
		// 电机在顶端, 夹子张开,晾衣杆在另一侧(Back),衣架闭合
		// 1. 电机在顶端
		Motor_Hang_Up() ;
		// 2. 夹子张开
		Servo_Claw_Open() ;
		// 3. 衣架闭合
		if (isSecond == false)
		{
			Servo_Hanger_Close() ;
			Servo_Hanger2_Close() ;
		}
		else
		{
			Servo_Hanger2_Close() ;
		}
		// 进入下一个状态
		HAL_Delay(1000) ;
		Robot_Status = Robot_Down ;
	}
	if (isSecond == false)
	{
		if (Robot_Status == Robot_Down && Robot_Wait_Cmd == 1)
		{
			// 1. 电机向下够衣服
			Motor_Hang_Down() ;
			// 进入下一个状态
			if (Is_Motor_Hanger_Down())
			{
				Robot_Status = Robot_Claw_Close ;
			}
		}
		else if (Robot_Status == Robot_Claw_Close)
		{
			// 1. 夹衣服
			Servo_Claw_Close() ;
			HAL_Delay(2000) ;
			// 进入下一个状态
			Robot_Status = Robot_Up ;
		}
		else if (Robot_Status == Robot_Up)
		{
			// 1. 电机上升,使得衣架能够穿过衣服
			Motor_Hang_Mid() ;
			// 进入下一个状态 
			if (Is_Motor_Hang_Mid())
			{
				Robot_Status = Robot_Hanger_Open ;
			}
		}
		else if (Robot_Status == Robot_Hanger_Open)
		{
			// 1. 衣架张开
			int goal = Servo_Hanger_Open() ;
			// 进入下一个状态 
			HAL_Delay(1500) ;
			Robot_Status = Robot_Claw_Open ;
		}
		else if (Robot_Status == Robot_Claw_Open)
		{
			// 1. 夹爪松开,放开衣服
			Servo_Claw_Open() ;
			HAL_Delay(1000) ;
			// 进入下一个状态 
			Robot_Status = Robot_Up_2 ;
		}
		else if (Robot_Status == Robot_Up_2)
		{
			// 1. 电机上升回位
			Motor_Hang_Up() ;
			// 进入下一个状态 
			if (IS_Motor_Hang_Up())
			{
				Robot_Status = Robot_SiGan_Next ;
			}
		}
		else if (Robot_Status == Robot_SiGan_Next)
		{ 
			// 1. 丝杆下一阶段
			Motor_SetAngle(&Motor_A	, 330) ;
			// 进入下一个状态
			if (Motor_Is_Angle(&Motor_A , 330, 20))
			{
				Robot_Status = Robot_OK ;
			}
		}
	}
//	if (Robot_Status == Robot_OK && isSecond == false)
//	{
//		HAL_Delay(1000) ;
//		isSecond = true ;
//		Robot_Status = Robot_Init ;
//	}
//	if (isSecond == true)
//	{
//		if (Robot_Status == Robot_Down)
//		{
//			// 1. 电机向下够衣服
//			Motor_Hang_Down() ;
//			// 进入下一个状态
//			if (Is_Motor_Hanger_Down())
//			{
//				Robot_Status = Robot_Claw_Close ;
//			}
//		}
//		else if (Robot_Status == Robot_Claw_Close)
//		{
//			// 1. 夹衣服
//			Servo_Claw_Close() ;
//			HAL_Delay(2000) ;
//			// 进入下一个状态
//			Robot_Status = Robot_Up ;
//		}
//		else if (Robot_Status == Robot_Up)
//		{
//			// 1. 电机上升,使得衣架能够穿过衣服
//			Motor_Hang_Mid() ;
//			// 进入下一个状态 
//			if (Is_Motor_Hang_Mid())
//			{
//				Robot_Status = Robot_Hanger_Open ;
//			}
//		}
//		else if (Robot_Status == Robot_Hanger_Open)
//		{
//			// 1. 衣架张开
//			int goal = Servo_Hanger2_Open() ;
//			// 进入下一个状态 
//			HAL_Delay(1500) ;
//			Robot_Status = Robot_Claw_Open ;
//		}
//		else if (Robot_Status == Robot_Claw_Open)
//		{
//			// 1. 夹爪松开,放开衣服
//			Servo_Claw_Open() ;
//			HAL_Delay(1000) ;
//			// 进入下一个状态 
//			Robot_Status = Robot_Up_2 ;
//		}
//		else if (Robot_Status == Robot_Up_2)
//		{
//			// 1. 电机上升回位
//			Motor_Hang_Up() ;
//			// 进入下一个状态 
//			if (IS_Motor_Hang_Up())
//			{
//				Robot_Status = Robot_SiGan_Next ;
//			}
//		}
//		else if (Robot_Status == Robot_SiGan_Next)
//		{ 
//			// 1. 丝杆下一阶段
//			Motor_SetAngle(&Motor_A	, 660) ;
//			// 进入下一个状态
//			if (Motor_Is_Angle(&Motor_A , 660, 20))
//			{
//				Robot_Status = Robot_OK ;
//			}
//		}
//	}
//	
//	
//	
//	

	// OLED展示效果
//	OLED_Printf(0, 20, OLED_6X8, "%d",Robot_Status) ;
//	OLED_Printf(0, 30, OLED_6X8, "M_A:%.2f M_B:%.2f",Motor_A.PID_Angle.realPoint_Now , Motor_B.PID_Angle.realPoint_Now) ;
	OLED_Printf(0,20,OLED_6X8 , "S1=%d S2=%d" ,Servo_1.current_pos , Servo_2.current_pos ) ;
	OLED_Printf(0,30,OLED_6X8 , "S2=%d S4=%d" ,Servo_3.current_pos , Servo_4.current_pos ) ;
	// Motor
	OLED_Printf(0,40,OLED_6X8 , "M_A:G=%.2f,R=%.2f" ,Motor_A.PID_Angle.goalPoint , Motor_A.PID_Angle.realPoint_Now ) ;
	OLED_Printf(0,50,OLED_6X8 , "M_B:G=%.2f,R=%.2f" ,Motor_B.PID_Angle.goalPoint , Motor_B.PID_Angle.realPoint_Now ) ;
}

void Mode_4_Tick(void)
{
	
}

void Mode_4_Exit(void)
{
  
}
