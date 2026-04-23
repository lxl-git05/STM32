#include "Con_Motor.h"

Motor_Typedef Motor_A;
Motor_Typedef Motor_B;

// 1. 所有电机初始化
void Con_Motor_init(void)
{
    Motor_Init
    (
        &Motor_A , &Motor_A_Encoder , &MyPWM_Motor_A_IN1 , &MyGPIO_Motor_A_IN1 , &MyGPIO_Motor_A_IN2 , Motor_AB_Param , Motor_DIR_P , Motor_DIR_P , 0.0f , 0.0f , 0.0f , 1000 , -1000 , 1000
    ) ;
    Motor_Init
    (
        &Motor_B , &Motor_B_Encoder , &MyPWM_Motor_B_IN1 , &MyGPIO_Motor_B_IN1 , &MyGPIO_Motor_B_IN2 , Motor_AB_Param , Motor_DIR_P , Motor_DIR_P , 0.0f , 0.0f , 0.0f , 1000 , -1000 , 1000
    ) ;
}

// 2. 设置目标速度
void Motor_SetGoalSpeed(Motor_Typedef *Motor , int speed)
{
	// 限制最值
	if (speed >= Motor->Motor_Param.MaxSpeed)
	{
		speed = Motor->Motor_Param.MaxSpeed ;
	}
	else if (speed < -(Motor->Motor_Param.MaxSpeed))
	{
		speed = -(Motor->Motor_Param.MaxSpeed) ;
	}
	// 判断方向,更新目标值
	Motor->PID_s.goalPoint = speed ;
	Motor->State = MOTOR_RUN;
}

// 3. 得到电机goal速度
int Motor_Get_GoalSpeed(Motor_Typedef *Motor)
{
    return Motor->PID_s.goalPoint ;
}

// 4. 电机停止
void Motor_Stop(Motor_Typedef *Motor)
{
    Motor->State = MOTOR_STOP;
}

// 5. 电机急刹
void Motor_Brake(Motor_Typedef *Motor)
{
    Motor->PID_s.goalPoint = 0;
    Motor->PID_s.setPoint = 0;
    Motor_SetPWM(Motor, 0);
    
    Motor->State = MOTOR_BRAKE;
}

// 6. 20ms电机状态更新
void Motor_Update_Tick(void)
{
    Motor_Speed_Update_Tick(&Motor_A , 20) ;
    Motor_Speed_Update_Tick(&Motor_B , 20) ;
}
