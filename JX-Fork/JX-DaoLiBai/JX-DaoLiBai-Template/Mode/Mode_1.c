#include "AllHeader.h"

// Mode1: 各个底层测试

void Mode_1_Setup(void)
{
	HAL_GPIO_WritePin(LED0_GPIO_Port , LED0_Pin , GPIO_PIN_RESET) ;	// 打开LED
}

// 1. ADC测试
void Mode1_RP_Check(void)
{
	// 1. ADC测试
	OLED_Printf(0,20,OLED_6X8,"ADC1=%d",RP_GetValue(1)) ;
	OLED_Printf(0,30,OLED_6X8,"ADC2=%d",RP_GetValue(2)) ;
	OLED_Printf(0,40,OLED_6X8,"ADC3=%d",RP_GetValue(3)) ;
	OLED_Printf(0,50,OLED_6X8,"ADC4=%d",RP_GetValue(4)) ;
}

// 2. Motor测试
int PWM_Check = 0 ;
void Mode1_Motor_Check(void)
{
	OLED_Printf(0,20,OLED_6X8,"PWM=%d",PWM_Check) ;
	Motor_SetPWM(PWM_Check) ;
}

// 3. 编码器测试
void Mode1_Encoder_Check(void)
{
	OLED_Printf(0,20,OLED_6X8,"Encoder:%d",Encoder_Get()) ;
}
// 4. 串口测试
void Mode1_Serial_Check(void)
{
	Serial_printf(&Serial1 , "%d,%d,%d,%d\n",RP_GetValue(1),RP_GetValue(2),RP_GetValue(3),RP_GetValue(4)) ;
}

// 5. 电机旋转角度测试
void Mode1_Encoder_Angle_Check(void)
{
	Encoder_Get() ;	// 刷新
	Motor_Angle_Update(&Motor) ;
	OLED_Printf(0,20,OLED_8X16,"Angle:%.2f",Motor.PID_Angle.realPoint_Now) ;
}

// 6. 测试电机速度
void Mode1_Encoder_Speed_Check(void)
{
	OLED_Printf(0,20,OLED_8X16,"Speed:%.2f",Motor.PID_s.realPoint_Now) ;
}
void Mode1_Encoder_Speed_Check_Tick(void)
{
	Motor_Speed_Update(&Motor , 20) ;
}

// 7. 倒立摆ADC读取
void Mode1_AD_ADC_Check(void)
{
	OLED_Printf(0,20,OLED_6X8,"AD=%d",AD_GetValue()) ;
}

// 代码测试
void Mode_1_Loop(void)
{
	
	OLED_Printf(0,0,OLED_8X16,"===Mode1===") ;
	// 1. ADC测试
//	Mode1_RP_Check() ;
	// 2. Motor测试
//	Mode1_Motor_Check() ;
	// 3. 编码器测试
//	Mode1_Encoder_Check() ;
	// 4. 串口测试
//	Mode1_Serial_Check() ;
	// 5. 电机旋转角度测试,中断最好关掉，因为后续中断可能有刷新，那么Encoder_Get就不能再次出现
//	Mode1_Encoder_Angle_Check() ;
	// 6. 测试电机速度
//	Mode1_Encoder_Speed_Check() ;
//	Motor_SetPWM(PWM_Check) ;
	// 7. 倒立摆ADC读取
	Mode1_AD_ADC_Check() ;
}

void Mode1_1ms_Tick(void)
{
}

void Mode1_5ms_Tick(void)
{
}

void Mode1_20ms_Tick(void)
{
//	Timer_Counter_Func() ;
//	Timer_Counter_Begin() ;
//	// 6. 测试电机速度
//	Mode1_Encoder_Speed_Check_Tick() ;
//	Timer_Counter_End() ;
}

void Mode_1_Exit(void)
{
	
}
