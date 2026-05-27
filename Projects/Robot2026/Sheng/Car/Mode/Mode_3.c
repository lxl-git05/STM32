#include "Mode_3.h"
#include "AllHeader.h"

// 测试全局变量
float check ;
int PWM_Servo_Check = 50;    // 50-250
int Servo_Pos_Check = 0 ;
int Servo_Pos_Check_Single[4] = {175,80,0,0} ;
 
// 测试函数声明
// 1. 测试串口功能
void Check_Serial(Serial_Typedef* pSerial) ;

// 2. 测试编码器 B 功能, 记得先初始化哦
void Check_Encoder(void) ;

// 3. 测试电机PWM(PID不准工作)
void Check_Motor_PWM(void) ;

// 4. 测试ESP32发送的指令
void Check_ESP32_Serial(void);

void Mode_3_Setup(void)
{
   OLED_Clear() ;
	 MyEncoder_Init(&Motor_A_Encoder) ;
	 MyEncoder_Init(&Motor_B_Encoder) ;
}

void Mode_3_Loop(void)
{
	OLED_Printf(0, 0, OLED_6X8, "=====Mode_3=====") ;
	// 本loop函数建议只执行一个check任务,防止未知Bug
	
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

// 4. 测试ESP32发送的指令
void Check_ESP32_Serial(void)
{
	// Serial2 串口2
	if (Serial_GetNewPackageFlag_ABC(&Serial2))
	{
		 // 1. 晾衣架开始 晾衣服
		 if (Serial_Check_Str(&Serial2 , "Hanger_Up"))
		 {
				Flash_Mode_Set(Flash_Mode_Fast) ;
		 }
		 // 2. 晾衣架开始 收衣服
		 if (Serial_Check_Str(&Serial2 , "Hanger_Down"))
		 {
				Flash_Mode_Set(Flash_Mode_OFF) ;
		 }
	}
}

void Mode_3_Exit(void)
{
    OLED_Clear() ;
}

void Mode_3_Tick(void)
{
	
}

