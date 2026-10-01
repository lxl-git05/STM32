#ifndef USER_INITIAL_H
#define USER_INITIAL_H

#define App_Task_Priority 		24U
#define UI_Task_Priority 		20U
#define Key_Task_Priority 		32U
#define Debug_Task_Priority 	24U
#define MPU6050_TASK_PRIORITY   32U

void Hardware_Init(void);
void Msg_Init(void);
void Task_Init(void);

#endif /* USER_INITIAL_H */
