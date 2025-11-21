#include "stm32f1xx.h"                  // Device header
#include "main.h"
#include "math.h"

// 实验变量
extern int check[] ;
extern int check2 ;
float a = 0.9f ;
int ActNow ;
int ActBef ;

// 调试变量
#define SumError_MAX 500	// 累次积分最大值
#define D_First ;	// 微分先行

float P_Out ;	// Kp项输出
float I_Out ;	// Ki项输出
float D_Out ;	// Kd项输出

float D_a = 0.0f ;	// 微分滤波系数
float I_a = 1  ;
float C = 1.0f ;

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

// 用来一般化初始化PID结构体
void PID_Init(Pid_Typedef *pid, float kp, float ki, float kd , float goalPoint)
{
	  pid->goalPoint = goalPoint;
	
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
}

// 用来确定各个PID的系数,调试专用
void PID_Set(Pid_Typedef *pid, float kp, float ki, float kd , float goalPoint)
{
	  pid->goalPoint = goalPoint;
	
    pid->Kp = kp;
    pid->Ki = ki;
    pid->Kd = kd;
}

float fx(int x)
{
	if (x < 20)
	{
		return -0.05f * x + 2 ;
	}
	else
	{
		return 0 ;
	}
}

// Speed_PID控制模版-目前来说发现速度PID调节更容易,累次积分限幅一个点即可
float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
{
	// 更新上次误差
	pid->LastError = pid->PreError;
	// 得到本次误差
	pid->PreError = pid->goalPoint - ActualValue;
	// 微分误差
	pid->dError = pid->PreError - pid->LastError;
	// 累次积分
	pid->SumError += pid->PreError ;
	// *积分限幅*
	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
	if (pid->SumError > SumError_MAX)
	{
		pid->SumError = SumError_MAX ;
	}
	else if (pid->SumError < -SumError_MAX)
	{
		pid->SumError = -SumError_MAX ;
	}
	
	// 输出参数
	P_Out = pid->Kp * pid->PreError ;
	I_Out = pid->Ki * pid->SumError ;
	D_Out = pid->Kd * pid->dError   ;
	
//	// 微分先行
//	D_Out = pid->Kd * (  D_a * pid->dError  - (1-D_a) * (pid->ActualPoint_Now - pid->ActualPoint_Before ) );
	
	// 数据采集
	check[0] = P_Out ;
	check[1] = I_Out ;
	check[2] = D_Out ;

	float Output = P_Out + I_Out + D_Out ;
	
	// 输出限幅
	if ( Output > OutputMax ) { Output = OutputMax ; }
	if ( Output < OutputMin ) { Output = OutputMin ; }
	
	return Output ;	// 新的设定值
}

//// Position_PID控制模版
//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// 积分累加:变速积分
//	C = fx(fabs(pid->PreError)) ;	// a的值自己测
//	// 积分累加:积分分离
//	if (fabs(pid->PreError) >= 20)
//	{
//		pid->SumError = 0 ;
//	}
//	// 积分累加:
//	pid->SumError += C * pid->PreError;
//	// 得到本次和上次实际值(更新)
//	pid->ActualPoint_Before = pid->ActualPoint_Now ;
//	pid->ActualPoint_Now = ActualValue ;
//	//*************** 实验区 **************** 
//	// 积分限幅
//	if (pid->SumError > SumError_MAX)
//	{
//		pid->SumError = SumError_MAX ;
//	}
//	else if (pid->SumError < -SumError_MAX)
//	{
//		pid->SumError = -SumError_MAX ;
//	}
//	
//	// 输出参数
//	P_Out = pid->Kp * pid->PreError ;
//	I_Out = pid->Ki * pid->SumError ;
//	D_Out = pid->Kd * pid->dError   ;
//	
//	// 微分先行
//	D_Out = pid->Kd * (  D_a * pid->dError  - (1-D_a) * (pid->ActualPoint_Now - pid->ActualPoint_Before ) );
//	
//	// 数据采集
//	check[0] = P_Out ;
//	check[1] = I_Out ;
//	check[2] = D_Out ;
//	
//	// ************** 结束 ***************
//	float Output = P_Out + I_Out + D_Out ;	// 输出值
//	
//	// 输出限幅-PWM的ARR
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}

//// 6. 输出偏移+输入死区
//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// 累次积分
//	pid->SumError += pid->PreError ;
//	// *积分限幅*
//	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
//	if (pid->SumError > 300)
//	{
//		pid->SumError = 300 ;
//	}
//	else if (pid->SumError < -300)
//	{
//		pid->SumError = -300 ;
//	}
//	// 计算PID: Out = Kp * Error0 + Ki * ErrorInt + Kd * ( Error0 - Error1 ) ;
//	float Output = pid->Kp * pid->PreError + pid->Ki * pid->SumError + pid->Kd * pid->dError ;
//	
//	// ************* 增加 ************** 
//	// 输出偏移-不能单独运行
////	if (Output > 0.1f)
////	{
////		Output += 6 ;
////	}
////	else if (Output < -0.1f)
////	{
////		Output -= 6 ;
////	}
////	else
////	{
////		Output = 0 ;
////	}
//	// 输出偏移+输入死区
//	if (fabs(pid->PreError) < 5)
//	{
//		Output = 0 ;
//	}
//	else
//	{
//		if (Output > 0.1f)
//		{
//			Output += 6 ;
//		}
//		else if (Output < -0.1f)
//		{
//			Output -= 6 ;
//		}
//		else
//		{
//			Output = 0 ;
//		}
//	}
//	
//	// ************* 结束 ************** 
//	// 输出限幅
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}


//// 5. 不完全微分(滤波):对于杂波,进行过滤,好处是变得平滑,坏处是位移延迟(毕竟与过去的值有关联)
//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// 累次积分
//	pid->SumError += pid->PreError ;
//	// *积分限幅*
//	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
//	if (pid->SumError > 300)
//	{
//		pid->SumError = 300 ;
//	}
//	else if (pid->SumError < -300)
//	{
//		pid->SumError = -300 ;
//	}
//	// 计算PID: Out = Kp * Error0 + Ki * ErrorInt + Kd * ( Error0 - Error1 ) ;
//	// ************* 删除 ************** 
//	
////	D_Out = pid->Kd * pid->dError ;
//	
//	// ************* 增加 ************** 
//	D_Out = (1.0f-a) * pid->Kd * pid->dError + a * D_Out ;
//	check[0] = D_Out * 100 ;
//	// ************* 结束 ************** 
//	
//	float Output = pid->Kp * pid->PreError + pid->Ki * pid->SumError + D_Out;
//	
//	// 输出限幅
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}



//// 4. 微分先行
//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// 累次积分
//	pid->SumError += pid->PreError ;
//	// *积分限幅*
//	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
//	if (pid->SumError > 300)
//	{
//		pid->SumError = 300 ;
//	}
//	else if (pid->SumError < -300)
//	{
//		pid->SumError = -300 ;
//	}
//	// 计算PID: Out = Kp * Error0 + Ki * ErrorInt + Kd * ( Error0 - Error1 ) ;
//	
//	// ************* 增加 **************
//	ActBef = ActNow ;
//	ActNow = ActualValue ;
//	
//	// 观测:
////	D_Out = pid->Kd * pid->dError ;	// 先前
//	D_Out =  - pid->Kd * (ActNow - ActBef) ;
//	check[0] = D_Out * 100 ;
//	
//	// 输出值
//	float Output = pid->Kp * pid->PreError + pid->Ki * pid->SumError + D_Out ;
//	
//	// ************* 结束 **************
//	// 输出限幅
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}


//// 3. 变速积分,误差大,系数小,误差小系数大
//// 系数大,过冲小,稳态慢 系数小,过冲大,稳态块
//// 观察发现,首先为了在差距很大的时候,是Kp在发挥作用,Ki也有作用
//// 距离逐渐变近,Kp
//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// ************** 删减 ***************
////	// 累次积分
////	pid->SumError += pid->PreError ;
//	
//	// ************* 增加 **************
//	C = 1.0f / (0.2 * 1.0f * fabs(pid->PreError) + 1.0f) ;
//	pid->SumError += C * pid->PreError;
//	
//	// ************* 结束 **************
//	// *积分限幅*
//	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
//	if (pid->SumError > 300)
//	{
//		pid->SumError = 300 ;
//	}
//	else if (pid->SumError < -300)
//	{
//		pid->SumError = -300 ;
//	}
//	// 计算PID: Out = Kp * Error0 + Ki * ErrorInt + Kd * ( Error0 - Error1 ) ;
//	float Output = pid->Kp * pid->PreError + pid->Ki * pid->SumError + pid->Kd * pid->dError ;
//	
//	// 输出限幅
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}


//// 2. 积分分离:(位置式更明显)
//// 对于速度和位置两个课题,速度需要持续提供力,所以Ki积分项会与摩擦力平衡,所以real在接近目标时不会先超调再减弱为target
//// 但是位置不一样,达到tar之后不需要再提供力或者是抵抗力,比如摩擦力,所以Ki的累次积分无处抵消,一定会导致超调,累次积分减小,最后回位

//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// ************** 删减 ***************
////	// 累次积分
////	pid->SumError += pid->PreError ;
//	
//	// ************* 增加 **************
//	// 积分分离:误差较小时放弃累次积分
//	if (pid->PreError > -50 && pid->PreError < 50)	// 在一个小区间才能积分累计
//	{
//		pid->SumError += pid->PreError;	// 继续累次积分
//	}
//	else
//	{
//		pid->SumError = 0 ;							// 不需要加入积分了,防止积分超调
//	}
//	// ************* 结束 **************
//	// *积分限幅*
//	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
//	if (pid->SumError > 300)
//	{
//		pid->SumError = 300 ;
//	}
//	else if (pid->SumError < -300)
//	{
//		pid->SumError = -300 ;
//	}
//	// 计算PID: Out = Kp * Error0 + Ki * ErrorInt + Kd * ( Error0 - Error1 ) ;
//	float Output = pid->Kp * pid->PreError + pid->Ki * pid->SumError + pid->Kd * pid->dError ;
//	
//	// 输出限幅
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}


//// 1. 原始:计算PID
//float PID_Cal(Pid_Typedef *pid, float ActualValue , float OutputMin , float OutputMax)
//{
//	// 更新上次误差
//	pid->LastError = pid->PreError;
//	// 得到本次误差
//	pid->PreError = pid->goalPoint - ActualValue;
//	// 微分误差
//	pid->dError = pid->PreError - pid->LastError;
//	// 累次积分
//	pid->SumError += pid->PreError ;
//	// *积分限幅*
//	// 方法:1.实测法,通过VOFA得到积分量的最大值,留点余量作为积分限幅 2.估算法:Output的MAX除以Ki(测好的正常值)即为限幅
//	if (pid->SumError > 300)
//	{
//		pid->SumError = 300 ;
//	}
//	else if (pid->SumError < -300)
//	{
//		pid->SumError = -300 ;
//	}
//	// 计算PID: Out = Kp * Error0 + Ki * ErrorInt + Kd * ( Error0 - Error1 ) ;
//	float Output = pid->Kp * pid->PreError + pid->Ki * pid->SumError + pid->Kd * pid->dError ;
//	
//	// 输出限幅
//	if ( Output > OutputMax ) { Output = OutputMax ; }
//	if ( Output < OutputMin ) { Output = OutputMin ; }
//	
//	return Output ;	// 新的设定值
//}
