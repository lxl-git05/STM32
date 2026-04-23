#ifndef __CON_MOTOR_H
#define __CON_MOTOR_H

#include "MySystem.h"
#include "Motor.h"

extern Motor_Typedef Motor_A;
extern Motor_Typedef Motor_B;

// 1. 所有电机初始化
void Con_Motor_init(void) ;

// 2. 设置目标速度
void Motor_SetGoalSpeed(Motor_Typedef *Motor , int speed) ;

// 3. 得到电机goal速度
int Motor_Get_GoalSpeed(Motor_Typedef *Motor) ;

// 4. 电机停止
void Motor_Stop(Motor_Typedef *Motor) ;

// 5. 电机急刹
void Motor_Brake(Motor_Typedef *Motor) ;

// 6. 20ms电机状态更新
void Motor_Update_Tick(void) ;

#endif // !
