#include "Motor.h"
#include "PWM.h"
#include "Encoder.h"

Motor_Typedef Motor ;
Motor_Param_Typedef Motor_Param = { 11.0f , 9.27666f , 500} ;

// 1. 初始化
void Motor_Init(void)
{
	Motor.Encoder_Cnt = 0;
	Motor.Motor_Param = &Motor_Param ;
	
	// PID环: 速度环PI 位置环PD
	PID_Init(&Motor.PID_s		  , 0.1325f , 4.65f , 0.0f , 100 , -100 , 20.0f , 0.020f) ;	// PWM最大值是+-100
	
	PWM_Init();
}

// 2. 配置PWM
void Motor_SetPWM(int8_t PWM)
{
	// PWM限幅
	if (PWM > 100)
	{
		PWM = 100 ;
	}
	else if (PWM < -100)
	{
		PWM = -100 ;
	}
	// PWM配置
	if (PWM >= 0)
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_RESET) ;
		HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_SET) ;
		PWM_SetCompare1(PWM);
	}
	else
	{
		HAL_GPIO_WritePin(AIN1_GPIO_Port , AIN1_Pin , GPIO_PIN_SET) ;
		HAL_GPIO_WritePin(AIN2_GPIO_Port , AIN2_Pin , GPIO_PIN_RESET) ;
		PWM_SetCompare1(-PWM);
	}
}

// 3. 得到一段周期内电机的速度,使用M法测速公式,得到Motor的转速:n圈/s
void Motor_Speed_Update(Motor_Typedef *Motor , uint32_t Gap_Time_ms)
{
	// 得到总脉冲数(含矫正方向)
	int Motor_CNT = Encoder_Get() ;
	
	// 转速n = 总脉冲数/倍频(4)/单圈脉冲数(11)/减速比(9.27666)/采样时间(Gap_Time_ms)
	Motor->PID_s.realPoint_Now = (float)Motor_CNT * 60 * 1000 / Gap_Time_ms /
		(4 * Motor->Motor_Param->PPR * Motor->Motor_Param->ReductionRatio)   ;
}

// 4. 得到当前电机旋转的角度
void Motor_Angle_Update(Motor_Typedef *Motor)
{
	// 得到角度 = 圈数 * 360
	float curr_Angle = (float)Encoder_cnt_Get() * 360.0f / 
		(4 * Motor->Motor_Param->PPR * Motor->Motor_Param->ReductionRatio)   ;
	
	// 记录当前角度
	Motor->PID_Angle.realPoint_Now = curr_Angle ;
}

// ======================= Func层 =======================
// 1. 设置电机goal速度
void Motor_SetSpeed(Motor_Typedef *Motor, float speed)
{
    if (speed >= Motor->Motor_Param->Motor_Max_Speed)
    {
        speed = Motor->Motor_Param->Motor_Max_Speed ;
    }
    else if (speed < -Motor->Motor_Param->Motor_Max_Speed)
    {
        speed = -Motor->Motor_Param->Motor_Max_Speed ;
    }
    Motor->PID_s.goalPoint = speed ;
}

// 2. 电机速度更新
void Motorx_Speed_Update_Tick(Motor_Typedef *Motor , uint32_t Gap_Time_ms)
{
    // 1. 计算真实速度（编码器）
    Motor_Speed_Update(Motor , Gap_Time_ms) ;
    // 2. PID计算
    PID_Update(&Motor->PID_s , Motor->PID_s.realPoint_Now) ;
    // 3. 输出PWM
    Motor_SetPWM(Motor->PID_s.setPoint);
}

// 3. 电机角度环PID,并不需要知道周期,但是仍然需要放在需要周期定时器内
void Motorx_Angle_Update_Tick(Motor_Typedef *Motor , int Dir)	// Dir: 纠正PID控制方向
{
	// 1. 计算角度
	Motor_Angle_Update(Motor) ;
	// 2. 计算PID
	PID_Update(&Motor->PID_Angle ,Motor->PID_Angle.realPoint_Now) ;
	// 3. 输出电机速度(串行环嵌套！！！)
	Motor_SetSpeed(Motor, Motor->PID_Angle.setPoint * Dir);
}

// 4. 电机角度环单环PID
void Motorx_Angle_Update_PWM_Tick(Motor_Typedef *Motor)
{
	// 0. 位置更新
	Encoder_Get() ;
	// 1. 计算角度
	Motor_Angle_Update(Motor) ;
	// 2. 计算PID
	PID_Update(&Motor->PID_Angle ,Motor->PID_Angle.realPoint_Now) ;
	// 3. 输出电机速度(串行环嵌套！！！)
	Motor_SetPWM(Motor->PID_Angle.setPoint) ;
} 

