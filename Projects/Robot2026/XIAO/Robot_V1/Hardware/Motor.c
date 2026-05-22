#include "Motor.h"

Motor_Param_Typedef Motor_AB_Param = {13.0f , 30.0f , 350 , 4} ;

// 1. 初始化
void Motor_Init(
	Motor_Typedef* pMotor , MyEncoder_Typedef* Motor_Encoder , MyPWM_Typedef* Motor_PWM , MyGPIO_Typedef* Motor_IN1_GPIO , MyGPIO_Typedef* Motor_IN2_GPIO , 
	Motor_Param_Typedef Motor_Param ,int8_t PWM_Dir , int8_t Encoder_Dir ,
	float kp, float ki, float kd , float OutMax , float OutMin , float ioutMax )
{
	// 填充参数
	pMotor->Motor_Encoder = Motor_Encoder ;
	pMotor->Motor_PWM = Motor_PWM ;
	pMotor->Motor_IN1_GPIO = Motor_IN1_GPIO ;
	pMotor->Motor_IN2_GPIO = Motor_IN2_GPIO ;
	pMotor->Motor_Param = Motor_Param ;
	pMotor->PWM_Dir = PWM_Dir ;
	pMotor->Encoder_Dir = Encoder_Dir ;
	pMotor->State = MOTOR_STOP ;
	PID_Init(&pMotor->PID_s, kp, ki, kd, OutMax, OutMin, ioutMax) ;
	// 初始化相关功能
	MyEncoder_Init(Motor_Encoder) ;
	MyPWM_Init(Motor_PWM) ;
}

// 2. 设置PWM,幕后执行的速度逻辑(setPoint)
void Motor_SetPWM(Motor_Typedef *Motor , int PWM)
{
	// PWM方向纠正
	PWM = PWM * Motor->PWM_Dir ;
	// 限制最值
	if (PWM >= Motor->Motor_PWM->PWM_MAX)
	{
		PWM = Motor->Motor_PWM->PWM_MAX ;
	}
	else if (PWM <= -Motor->Motor_PWM->PWM_MAX)
	{
		PWM = -Motor->Motor_PWM->PWM_MAX ;
	}
	// 判断方向,设置速度
	if (PWM >= 0)
	{
		My_GPIO_WritePin(Motor->Motor_IN1_GPIO , 0) ;	// 低电平
		MyPWM_SetCompare(Motor->Motor_PWM , PWM) ;		// 变大,差值变大
	}
	else
	{
		My_GPIO_WritePin(Motor->Motor_IN1_GPIO , 1) ;	// 高电平
		MyPWM_SetCompare(Motor->Motor_PWM ,Motor->Motor_PWM->PWM_MAX + PWM) ;// 变小  ,差值变大
	}
}

// 3. 使用M法测速公式,得到Motor的转速:n圈/s , 自己配置计时器
void Motor_Speed_Update(Motor_Typedef *Motor , uint32_t sample_time_ms)
{
	// 得到总脉冲数
	int Motor_CNT = MyEncoder_Get_CNT(Motor->Motor_Encoder) * Motor->Encoder_Dir;
	
	// 转速rpm = 总脉冲数/4倍频/单圈脉冲数(13)/减速比(30)/采样时间 , sample_time_ms 一般为20ms
	Motor->PID_s.realPoint_Now = (float)Motor_CNT * 60 * 1000 / sample_time_ms / 
	(Motor->Motor_Param.PPR * Motor->Motor_Param.ReductionRatio * Motor->Motor_Param.Timer_Freq)  ;
}

// 4. 电机状态更新和PID控制全流程
void Motor_Speed_Update_Tick(Motor_Typedef *Motor , uint32_t sample_time_ms)
{
    // 1. 计算真实速度（编码器）
    Motor_Speed_Update(Motor , sample_time_ms) ;

    // 2. 状态机控制
    switch (Motor->State)
    {
        case MOTOR_STOP:    // 停车
            Motor->PID_s.goalPoint = 0;
            break;

        case MOTOR_RUN:     // 行进
            break;

        case MOTOR_BRAKE:   // 刹车
						Motor->PID_s.goalPoint = 0;
						Motor->PID_s.setPoint  = 0;
            Motor_SetPWM(Motor, 0);
            return;
    }

    // 3. PID计算
    PID_Update(&Motor->PID_s , Motor->PID_s.realPoint_Now) ;

    // 4. 输出PWM
    Motor_SetPWM(Motor, Motor->PID_s.setPoint);
}
