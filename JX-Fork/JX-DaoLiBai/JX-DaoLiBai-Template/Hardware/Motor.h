#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "MyPID.h"

// 定义Motor结构体
// 电机自身参数
typedef struct
{
	float PPR;           	// 编码器线数
	float ReductionRatio;	// 减速比
	int Motor_Max_Speed ;	// 电机最大速度
}Motor_Param_Typedef ;

// 电机参数:本工程只需要一个电机，并且不考虑可拓展性,所以简单定义
typedef struct
{
	// 1. Encoder
	int32_t Encoder_Cnt ;
	// 2. Param
	Motor_Param_Typedef* Motor_Param ;
	// 3. PID
	Pid_Typedef PID_s ;				// 速度环(一般为内环)
	Pid_Typedef PID_Angle ;		// 角度环(一般为外环)

}Motor_Typedef ;

extern Motor_Typedef Motor ; 

void Motor_Init(void) ;
void Motor_SetPWM(int8_t PWM) ;
// 3. 得到一段周期内电机的速度,使用M法测速公式,得到Motor的转速:n圈/s
void Motor_Speed_Update(Motor_Typedef *Motor , uint32_t Gap_Time_ms) ;
// 4. 得到当前电机旋转的角度
void Motor_Angle_Update(Motor_Typedef *Motor) ;

// ======================= Func层 =======================
// 1. 设置电机goal速度
void Motor_SetSpeed(Motor_Typedef *Motor, float speed) ;
// 2. 电机速度更新
void Motorx_Speed_Update_Tick(Motor_Typedef *Motor , uint32_t Gap_Time_ms) ;
// 3. 电机角度环PID,并不需要知道周期,但是仍然需要放在需要周期定时器内
void Motorx_Angle_Update_Tick(Motor_Typedef *Motor , int Dir);	// Dir: 纠正PID控制方向

#endif
