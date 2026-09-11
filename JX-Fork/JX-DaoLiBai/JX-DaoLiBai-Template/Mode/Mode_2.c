#include "AllHeader.h"

// Mode2: 单环：速度环PID

void Mode_2_Setup(void)
{
	
}

void Mode_2_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"===Mode2===") ;
	// 串口配置PID和速度
	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &Motor.PID_s.Kp);
	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &Motor.PID_s.Ki);
	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &Motor.PID_s.Kd);
	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &Motor.PID_s.goalPoint);
	// 串口打印变量
	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n", Motor.PID_s.goalPoint, Motor.PID_s.realPoint_Now, Motor.PID_s.setPoint);
}

void Mode2_1ms_Tick(void)
{
}

void Mode2_5ms_Tick(void)
{
}

void Mode2_20ms_Tick(void)
{
	Motorx_Speed_Update_Tick(&Motor , 20) ;
}

void Mode_2_Exit(void)
{
}
