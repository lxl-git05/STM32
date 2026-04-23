#ifndef __CON_SERVO_H
#define __CON_SERVO_H

#include "Servo.h"

extern Servo_Typedef Servo_1 ;
extern Servo_Typedef Servo_2 ;
extern Servo_Typedef Servo_3 ;
extern Servo_Typedef Servo_4 ;

void Con_Servo_Init(void) ;
void Con_Servo_GoalAngle_Tick(void) ;

#endif
