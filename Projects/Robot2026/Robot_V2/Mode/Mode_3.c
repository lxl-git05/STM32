#include "Mode_3.h"
#include "AllHeader.h"

// 测试全局变量
float check ;
int PWM_Servo_Check = 50;    // 50-250
int Servo_Pos_Check = 0 ;
int Servo_Pos_Check_Single[4] ;
 
// 测试函数声明
// 1. 测试串口功能
void Check_Serial(Serial_Typedef* pSerial) ;

// 2. 测试编码器 B 功能, 记得先初始化哦
void Check_Encoder(void) ;

// 3. 测试电机PWM(PID不准工作)
void Check_Motor_PWM(void) ;

// 4. 同时测试所有Servo的PWM功能, 记得先初始化哦
void Check_Servo(void) ;

void Mode_3_Setup(void)
{
   OLED_Clear() ;
   OLED_Printf(0, 0, OLED_6X8, "=====Mode_3=====") ;
	 MyEncoder_Init(&Motor_A_Encoder) ;
	 MyEncoder_Init(&Motor_B_Encoder) ;
}

void Mode_3_Loop(void)
{
	// 本loop函数建议只执行一个check任务,防止未知Bug
//	Check_Serial(&Serial2) ;
	Check_Servo() ;
}

// 1. 测试串口功能
void Check_Serial(Serial_Typedef* pSerial)
{
    if (Serial_GetNewPackageFlag_ABC(pSerial))
    {
        Serial_SetFloatData(pSerial, "Kp", "Kp=%f", &check) ;
				Serial_SetFloatData(pSerial, "Ki", "Ki=%f", &check) ;
				Serial_SetFloatData(pSerial, "Kd", "Kd=%f", &check) ;
        Serial_printf(pSerial , "%f\n", check) ;
    }
    OLED_Printf(0, 40, OLED_6X8, "%.2f" , check) ;  // 测试OLED
}

// 2. 测试编码器功能, 记得先初始化哦
void Check_Encoder(void)
{
    OLED_Printf( 0, 40, OLED_6X8, "%d" , MyEncoder_Get_CNT(&Motor_A_Encoder)) ;
		OLED_Printf(20, 40, OLED_6X8, "%d" , MyEncoder_Get_CNT(&Motor_B_Encoder)) ;
}
// 3. 测试电机PWM(PID不准工作)
void Check_Motor_PWM(void)
{
		static int Motor_PWM_Check = 0;
		Motor_A.PID_s.Kp = 0.0f ;
		Motor_A.PID_s.Ki = 0.0f ;
		Motor_A.PID_s.Kd = 0.0f ;
	  if (Key_Check(KEY_1, KEY_SINGLE))
    {
        Motor_PWM_Check += 400 ;
        if (Motor_PWM_Check > 1000)
        {
            Motor_PWM_Check = 0 ;
        }
    }
		
		OLED_ClearArea(0,20,128,10) ;
		OLED_Printf(0,20,OLED_6X8 , "Motor_Speed:%d" , Motor_PWM_Check) ;
		Motor_SetPWM(&Motor_A ,Motor_PWM_Check) ;
}


// 4. 同时测试所有Servo的PWM功能, 记得先初始化哦
void Check_Servo(void)
{
		if (Serial_GetNewPackageFlag_ABC(&Serial1))
    {
        Serial_SetIntData(&Serial1, "Angle", "Angle=%d", &Servo_Pos_Check) ;
    }
		
//    Servo_SetDirectAngle(&Servo_1 , Servo_Pos_Check) ;	// 146加紧 164张开
//		Servo_SetDirectAngle(&Servo_2 , Servo_Pos_Check) ;	// 73加紧，48张开 
//		Servo_SetDirectAngle(&Servo_3 , Servo_Pos_Check) ;
		Servo_SetDirectAngle(&Servo_4 , Servo_Pos_Check) ;
//		
		// 舵机控制:夹爪
		static bool Claw_Status = true	;
		if (Key_Check(KEY_2 , KEY_SINGLE))
		{
			Claw_Status = !Claw_Status ;
		}
		if (Claw_Status)
		{
			Servo_Claw_Open() ;
		}
		else 
		{
			Servo_Claw_Close() ;
		}
		// 舵机控制：衣架
		static bool Hanger_Status = true	;
		if (Key_Check(KEY_2 , KEY_LONG))
		{
			Hanger_Status = !Hanger_Status ;
		}
		if (Hanger_Status)
		{
			Servo_Hanger_Close() ;
		}
		else
		{
			Servo_Hanger_Open() ;
		}
		OLED_ClearArea(0,20,128,10) ;OLED_ClearArea(0,30,128,10) ;
		OLED_ClearArea(0,40,128,10) ;OLED_ClearArea(0,50,128,10) ;
		
		OLED_Printf(0,20,OLED_6X8 , "Servo1_Pos  =  %d" ,Servo_1.current_pos ) ;
		OLED_Printf(0,30,OLED_6X8 , "Servo2_Pos  =  %d" ,Servo_2.current_pos ) ;
		OLED_Printf(0,40,OLED_6X8 , "Servo3_Pos  =  %d" ,Servo_3.current_pos ) ;
		OLED_Printf(0,50,OLED_6X8 , "Servo4_Pos  =  %d" ,Servo_4.current_pos ) ;
		
//		Servo_SetDirectAngle(&Servo_1 , Servo_Pos_Check_Single[0]) ;
//		Servo_SetDirectAngle(&Servo_2 , Servo_Pos_Check_Single[1]) ;
//		Servo_SetDirectAngle(&Servo_3 , Servo_Pos_Check_Single[2]) ;
//		Servo_SetDirectAngle(&Servo_4 , Servo_Pos_Check_Single[3]) ;
}

void Mode_3_Exit(void)
{
    OLED_Clear() ;
}

void Mode_3_Tick(void)
{
	
}

