#include "AllHeader.h"

// 倒立摆
uint8_t State = 0 ;				// 是否启动
uint16_t Angle ;					// 读取角度
#define CENTER_ANGLE 2060	// 倒立摆接近中心
#define CENTER_RANGE 800	// 倒立摆可调范围
// 倒立摆PID定义
Pid_Typedef PID_AD ;			// 角度环(内环)，外环为Motor外环(也就是Angle环)

void Mode_5_Setup(void)
{
	PID_Init(&PID_AD , 0.23f , 3.08f , 0.00943f , 100 , -100 , 5.0f , 0.005f) ;
	PID_AD.goalPoint = CENTER_ANGLE ;

//	Motor.PID_Angle.d_style = 1.0f ;
//	Motor.PID_Angle.d_filter = 0.9f ;
	PID_Init(&Motor.PID_Angle , 0.622f , 0.152f , 0.345f , 100 , -100 , 50.0f , 0.050f) ;			// 速度实测能到600+,但是这里还是限制500吧
}

void Mode_5_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"===Mode5===") ;
	// 按键切换倒立摆是否使能
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		if (State != 4)
		{
			State = 4 ;	// 暂时是4而不是起摆
		}
		else
		{
			State = 0 ;	
		}
	}
	// LED指示倒立摆状态
	if ( State == 4) {HAL_GPIO_WritePin(LED0_GPIO_Port , LED0_Pin ,GPIO_PIN_RESET) ;}	 // 开启->亮
	if ( State != 4) {HAL_GPIO_WritePin(LED0_GPIO_Port , LED0_Pin ,GPIO_PIN_SET  ) ;}  // 关闭->灭
	// OLED展示
	OLED_Printf(0,20,OLED_6X8,"AD=%04d",Angle) ;
	// 串口调试
	// 1. 内环
//	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &PID_AD.Kp);
//	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &PID_AD.Ki);
//	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &PID_AD.Kd);
//	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &PID_AD.goalPoint);
//	// 串口打印变量
//	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n", PID_AD.goalPoint, PID_AD.realPoint_Now, PID_AD.setPoint);
		// 2. 外环
//	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &Motor.PID_Angle.Kp);
//	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &Motor.PID_Angle.Ki);
//	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &Motor.PID_Angle.Kd);
//	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &Motor.PID_Angle.goalPoint);
//	// 串口打印变量
//	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n", Motor.PID_Angle.goalPoint, Motor.PID_Angle.realPoint_Now, Motor.PID_Angle.setPoint);
}

void Mode5_1ms_Tick(void)
{
	// 角度读取
	Angle = AD_GetValue() ;
	if (!(Angle > CENTER_ANGLE - CENTER_RANGE && Angle < CENTER_ANGLE + CENTER_RANGE))	// 角度在可调范围之外
	{
		State = 0 ;	// 赶快关掉
	}
	// 关闭的时候PWM为0
	if (State == 0)
	{
		Motor_SetPWM(0) ;
	}
}

void Mode5_5ms_Tick(void)
{
	if (State == 4)
	{
		// 内环PID：倒立摆角度环
		// 1. 读取当前角度(1ms已经完成)

		// 2. 进行PID计算(目标已经有了，当前是上一步，设定是下一步)
		PID_Update(&PID_AD , Angle) ;
		
		// 3. 设定值配置
		Motor_SetPWM(PID_AD.setPoint) ;
	}
}

void Mode5_20ms_Tick(void)
{
	Timer_Counter_Func() ;
	if (State == 4)
	{
		// 外环PID: 倒立摆位置环
		// 1. 计算角度
		Encoder_Get() ;
		Motor_Angle_Update(&Motor) ;
		// 2. 计算PID
		PID_Update(&Motor.PID_Angle ,Motor.PID_Angle.realPoint_Now) ;
		// 3. 输出角度目标
		PID_AD.goalPoint = CENTER_ANGLE - Motor.PID_Angle.setPoint ;
	}
}

void Mode_5_Exit(void)
{
}
