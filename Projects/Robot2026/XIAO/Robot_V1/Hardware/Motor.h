#ifndef __MOTOR_H
#define __MOTOR_H

#include "MySystem.h"
#include "MyPID.h"

// 电机正方向配置
#define Motor_DIR_P ( 1)
#define Motor_DIR_N (-1)

// 电机自身参数
typedef struct
{
	float PPR;           	// 编码器线数
	float ReductionRatio;	// 减速比
	int MaxSpeed ;				// 最大速度(rpm)
	uint8_t Timer_Freq ;	// 倍频
}Motor_Param_Typedef ;

// 电机状态枚举
typedef enum
{
    MOTOR_STOP = 0, // 停车
    MOTOR_RUN,      // 允许发车
    MOTOR_BRAKE     // 急刹车
}Motor_State_Typedef;

// 电机参数,包含寻址的参数都必须使用指针!!!
typedef struct
{
	// 编码器配置
	MyEncoder_Typedef* Motor_Encoder ;
	// PWM配置
	MyPWM_Typedef* Motor_PWM ;
	// GPIO配置
	MyGPIO_Typedef* Motor_IN1_GPIO ;
	MyGPIO_Typedef* Motor_IN2_GPIO ;
	// 电机自身参数
	Motor_Param_Typedef Motor_Param ;	
	// PID参数
	Pid_Typedef PID_s ;	
	// 方向参数
	int8_t PWM_Dir;         			// PWM配置正方向(PWM为正对应向前为正方向)
	int8_t Encoder_Dir ;				// 编码器正方向(向前,编码器++为正方向)
	// 电机状态
	Motor_State_Typedef State;			// 电机状态
}Motor_Typedef ;

extern Motor_Param_Typedef Motor_AB_Param ;

// 1. 初始化
void Motor_Init
(
	Motor_Typedef* pMotor , MyEncoder_Typedef* Motor_Encoder , MyPWM_Typedef* Motor_PWM , MyGPIO_Typedef* Motor_IN1_GPIO , MyGPIO_Typedef* Motor_IN2_GPIO , 
	Motor_Param_Typedef Motor_Param ,int8_t PWM_Dir , int8_t Encoder_Dir ,
	float kp, float ki, float kd , float OutMax , float OutMin , float ioutMax 
) ;

// 2. 设置PWM,幕后执行的速度逻辑(setPoint)
void Motor_SetPWM(Motor_Typedef *Motor , int PWM) ;

// 3. 电机状态更新和PID控制全流程
void Motor_Speed_Update_Tick(Motor_Typedef *Motor , uint32_t sample_time_ms) ;
void Motor_Speed_Update(Motor_Typedef *Motor , uint32_t sample_time_ms) ;
#endif
