#include "Mode_6.h"
#include "AllHeader.h"

extern int Base_Speed ;
extern Pid_Typedef PID_ALL_Pos ;

float goal_mpu_pos = 0 ;

// setup
void Mode_6_Setup(void)
{
   OLED_Clear() ;
}

// loop
void Mode_6_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"==Mode_Pos_MPU==") ;
	// Serial参数更改
	if (Serial_GetNewPackageFlag_ABC(&Serial1))
	{
			// 得到数据
			Serial_SetFloatData(&Serial1, "Kp", "Kp=%f", &PID_ALL_Pos.Kp) ;
			Serial_SetFloatData(&Serial1, "Ki", "Ki=%f", &PID_ALL_Pos.Ki) ;
			Serial_SetFloatData(&Serial1, "Kd", "Kd=%f", &PID_ALL_Pos.Kd) ;
			Serial_SetFloatData(&Serial1, "Angle", "Angle=%f", &goal_mpu_pos) ;
	}
	PID_ALL_Pos_Set_Goal(goal_mpu_pos) ;
	// OLED展示
	OLED_Printf(0,20,OLED_6X8 , "Pos: A: %.4f", Motor_A.PID_Pos.realPoint_Now) ;
	OLED_Printf(0,30,OLED_6X8 , "Pos: B: %.4f", Motor_B.PID_Pos.realPoint_Now) ;
	OLED_Printf(0,40,OLED_6X8 , "Pos: All: %.4f", PID_ALL_Pos.realPoint_Now) ;
	OLED_Printf(0,50,OLED_6X8 , "yaw:    %.4f", MPU_Real.yaw) ;
}

// 中断控制器
void Mode_6_Tick(void)
{
	PID_Angle_Tick(PID_ALL_Pos_Tick()) ;	// 外环:小车走直线，并且配置 BaseSpeed 基础速度
	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n",PID_ALL_Pos.goalPoint ,PID_ALL_Pos.realPoint_Now ,PID_ALL_Pos.setPoint );
}

void Mode_6_Exit(void)
{
  
}
