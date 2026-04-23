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
	Servo_Init(&Servo_4, &MyPWM_Servo4, SERVO_TYPE_180 ,50, 250 ,0 , 180 , 0 , 3);
}

void Con_Servo_GoalAngle_Tick(void)
{
	Servox_GoalAngle_Tick(&Servo_1) ;
	Servox_GoalAngle_Tick(&Servo_2) ;
	Servox_GoalAngle_Tick(&Servo_3) ;
	Servox_GoalAngle_Tick(&Servo_4) ;
}

// 夹爪张开 小 60一般 75加紧 大,越大越张开 左
void Servo_Claw_Open(void)
{
	Servo_SetDirectAngle(&Servo_3 , 75) ;
	Servo_SetDirectAngle(&Servo_4 , 75) ;
}

// 夹爪闭合
void Servo_Claw_Close(void)
{
	Servo_SetDirectAngle(&Servo_3 , 90) ;
	Servo_SetDirectAngle(&Servo_4 , 90) ;
}


