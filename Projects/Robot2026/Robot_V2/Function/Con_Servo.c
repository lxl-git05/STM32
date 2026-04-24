#include "Con_Servo.h"
#include "Servo.h"

Servo_Typedef Servo_1 ;
Servo_Typedef Servo_2 ;
Servo_Typedef Servo_3 ;
Servo_Typedef Servo_4 ;

void Con_Servo_Init(void)
{
	// 																													[pos限幅] Init step
	Servo_Init(&Servo_1, &MyPWM_Servo1, SERVO_TYPE_180 ,50, 250 ,0 , 180 , 0 , 3);
	Servo_Init(&Servo_2, &MyPWM_Servo2, SERVO_TYPE_180 ,50, 250 ,0 , 180 , 0 , 3);
	
	Servo_Init(&Servo_3, &MyPWM_Servo3, SERVO_TYPE_180 ,50, 250 ,0 , 180 , 0 , 3);
	Servo_Init(&Servo_4, &MyPWM_Servo4, SERVO_TYPE_180 ,50, 250 ,0 , 180 , 0 , 1);
}

void Con_Servo_GoalAngle_Tick(void)
{
	Servox_GoalAngle_Tick(&Servo_1) ;
	Servox_GoalAngle_Tick(&Servo_2) ;
	Servox_GoalAngle_Tick(&Servo_3) ;
	Servox_GoalAngle_Tick(&Servo_4) ;
}

// 夹爪张开
void Servo_Claw_Open(void)
{
	Servo_SetDirectAngle(&Servo_1 , 175) ;	// 135 加紧, 175 张开
	Servo_SetDirectAngle(&Servo_2 , 70)  ;	//  73 加紧,  48 张开 
}

// 夹爪闭合
void Servo_Claw_Close(void)
{
	Servo_SetDirectAngle(&Servo_1 , 145) ;
	Servo_SetDirectAngle(&Servo_2 , 84) ;	// 112
}

// 衣架 闭合 95 °
int Servo_Hanger_Close(void)
{
	int target = 46 ;
	Servo_SetDirectAngle(&Servo_3 , target) ;
	return target ;
}

// 衣架 张开 0 °
int Servo_Hanger_Open(void)
{
	int target = 158 ;
	Servo_SetDirectAngle(&Servo_3 , target) ;
	return target ;
}

// 机械臂ARM 94 竖直 , 73 碰壁
// 机械臂 竖直
int Servo_Arm_Come(void)
{
//	Servo_SetDirectAngle(&Servo_4 , 99) ;
	int target = 99 ;
	Servo_SetGoalAngle(&Servo_4 , target) ;
	return target ;
}

// 机械臂 碰壁(back)
int Servo_Arm_Back(void)
{
//	Servo_SetDirectAngle(&Servo_4 , 72) ;
	int target = 72 ;
	Servo_SetGoalAngle(&Servo_4 , target) ;
	return target ;
}


