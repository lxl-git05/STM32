#ifndef __PID_H__
#define __PID_H__

//typedef struct PID{
//		float goalPoint;			
//		
//		float P;						
//		float I;						
//		float D;						
//		
//		float LastError;		
//		float PreError;			
//		float SumError;			
//		float dError;
//	
//		float IMax;					
//		
//		float POut;					
//		float IOut;					
//		float DOut;					
//}Pid_Typedef;

typedef struct PID
{
	float goalPoint;		// 目标参数	

	float Kp;			// 比例系数				
	float Ki;			// 积分系数	
	float Kd;			// 微分系数		

	float LastError;	// 上次的误差
	float PreError;		// 本次误差
	float SumError;		// 积分误差(误差和)	
	float dError;		// 微分误差(本次-上次)
	
	// 新增变量
	float ActualPoint_Now 	 ;	// 当前真实值
	float ActualPoint_Before ;	// 上次真实值
	
}Pid_Typedef;

void PID_Init(Pid_Typedef *pid, float kp, float ki, float kd ,  float goalPoint) ;
float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax) ;
void PID_Set(Pid_Typedef *pid, float kp, float ki, float kd , float goalPoint) ;


#endif
