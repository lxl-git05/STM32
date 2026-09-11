#include "AllHeader.h"

// Mode4: 单环：位置环PID

void Mode_4_Setup(void)
{
	Motor.PID_Angle.Kp = 1.123f ;
	Motor.PID_Angle.Ki = 0 ;
	Motor.PID_Angle.Kd = 0.928f ;
	
	Motor.PID_Angle.OutMax = 100 ;
	Motor.PID_Angle.OutMin =-100 ;
}

void Mode_4_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"===Mode4===") ;
	// 串口配置PID和速度
	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &Motor.PID_Angle.Kp);
	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &Motor.PID_Angle.Ki);
	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &Motor.PID_Angle.Kd);
	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &Motor.PID_Angle.goalPoint);
	// 串口打印变量
	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n", Motor.PID_Angle.goalPoint, Motor.PID_Angle.realPoint_Now, Motor.PID_Angle.setPoint);
}

void Mode4_1ms_Tick(void)
{
}

void Mode4_5ms_Tick(void)
{
}

void Mode4_20ms_Tick(void)
{
	// 角度单环控制PWM
	Motorx_Angle_Update_PWM_Tick(&Motor) ;
}

void Mode_4_Exit(void)
{
}
