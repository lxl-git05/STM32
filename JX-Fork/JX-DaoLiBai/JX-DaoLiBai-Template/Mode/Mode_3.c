#include "AllHeader.h"

// Mode3: 双环：外环: 位置环PID 内环: 速度环PID
// 只需要配置Tick即可

void Mode_3_Setup(void)
{
	// 因为是双环,所以相对PWM环,参数会变
	Motor.PID_Angle.Kp = 3.24f ;
	Motor.PID_Angle.Ki = 0 ;
	Motor.PID_Angle.Kd = 0.031f ;
	
	Motor.PID_Angle.OutMax = 500 ;	// 驱动的是真实速度(500)，而不是PWM(100)
	Motor.PID_Angle.OutMin =-500 ;
}

void Mode_3_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"===Mode3===") ;
	// 串口配置PID和速度
	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &Motor.PID_Angle.Kp);
	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &Motor.PID_Angle.Ki);
	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &Motor.PID_Angle.Kd);
	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &Motor.PID_Angle.goalPoint);
	// 串口打印变量
	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n", Motor.PID_Angle.goalPoint, Motor.PID_Angle.realPoint_Now, Motor.PID_Angle.setPoint);
}

void Mode3_1ms_Tick(void)
{
}

void Mode3_5ms_Tick(void)
{
}

void Mode3_20ms_Tick(void)
{
	// 一共40us
	Timer_Counter_Begin() ;
	// 外环: 位置环
	Motorx_Angle_Update_Tick(&Motor ,  1) ;
	// 内环: 速度环
	Motorx_Speed_Update_Tick(&Motor , 20) ;
	Timer_Counter_End() ;
}

void Mode_3_Exit(void)
{
}
