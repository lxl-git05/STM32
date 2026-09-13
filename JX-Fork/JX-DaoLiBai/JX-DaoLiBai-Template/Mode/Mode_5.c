#include "AllHeader.h"

// 倒立摆
uint8_t State = 0 ;				// 是否启动
uint16_t Angle ;					// 读取角度
#define CENTER_ANGLE 2070	// 倒立摆接近中心
#define CENTER_RANGE 800	// 倒立摆可调范围
#define START_PWM 35
#define START_TIME 100
// 倒立摆PID定义
Pid_Typedef PID_AD ;			// 角度环(内环)，外环为Motor外环(也就是Angle环)

static uint16_t Count0, CountTime;
static uint16_t Angle0, Angle1, Angle2;

void Mode_5_Setup(void)
{
	PID_Init(&PID_AD , 0.20f , 2.5f , 0.00762f , 100 , -100 , 5.0f , 0.005f) ;
	PID_AD.goalPoint = CENTER_ANGLE ;
	PID_AD.d_filter = 0.0f ;

//	Motor.PID_Angle.d_style = 1.0f ;
//	Motor.PID_Angle.d_filter = 0.9f ;
//	PID_Init(&Motor.PID_Angle , 0.622f , 0.152f , 0.345f , 100 , -100 , 50.0f , 0.050f) ;			// 速度实测能到600+,但是这里还是限制500吧
	PID_Init(&Motor.PID_Angle , 0.321f , 0.27f , 0.353f , 100 , -100 , 50.0f , 0.050f) ;
	
//	Motor.PID_Angle.d_style  = 1.0f;
//	Motor.PID_Angle.d_filter = 0.55f;

	State = 0 ;
	Count0 = 0 ;
	CountTime = 0 ;
	Angle0 = 0 ;
	Angle1 = 0 ;
	Angle2 = 0 ;
}

void Mode_5_Loop(void)
{
	OLED_Printf(0,0,OLED_8X16,"===Mode5===") ;
	// 按键切换倒立摆是否使能
	if (Key_Check(KEY_1 , KEY_SINGLE))
	{
		if (State == 0)
		{
			State = 21 ;
		}
		else
		{
			State = 0 ;	
		}
	}
	// LED指示倒立摆状态
	if ( State != 0) {HAL_GPIO_WritePin(LED0_GPIO_Port , LED0_Pin ,GPIO_PIN_RESET) ;}	 // 开启->亮
	if ( State == 0) {HAL_GPIO_WritePin(LED0_GPIO_Port , LED0_Pin ,GPIO_PIN_SET  ) ;}  // 关闭->灭
	// OLED展示
	OLED_Printf(0,20,OLED_6X8,"AD=%04d",Angle) ;
	OLED_Printf(96,0,OLED_6X8,"%02d",State) ;
	// 串口调试
	// 1. 内环
//	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &PID_AD.Kp);
//	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &PID_AD.Ki);
//	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &PID_AD.Kd);
//	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &PID_AD.goalPoint);
//	// 串口打印变量
//	Serial_printf(&Serial1, "%.2f,%.2f,%.2f\n", PID_AD.goalPoint, PID_AD.realPoint_Now, PID_AD.setPoint);
		// 2. 外环
	Serial_SetFloatData(&Serial1, "Kp",   "Kp=%f",   &Motor.PID_Angle.Kp);
	Serial_SetFloatData(&Serial1, "Ki",   "Ki=%f",   &Motor.PID_Angle.Ki);
	Serial_SetFloatData(&Serial1, "Kd",   "Kd=%f",   &Motor.PID_Angle.Kd);
	Serial_SetFloatData(&Serial1, "Goal", "Goal=%f", &Motor.PID_Angle.goalPoint);
	// 串口打印变量
	Serial_printf(&Serial1, "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", Motor.PID_Angle.goalPoint, Motor.PID_Angle.realPoint_Now, Motor.PID_Angle.setPoint,Motor.PID_Angle.pout,Motor.PID_Angle.iout,Motor.PID_Angle.dout);
}

void Mode5_1ms_Tick(void)
{
	// 角度读取
	Angle = AD_GetValue() ;
	Encoder_Get() ;

	if (State == 0)
	{
		Motor_SetPWM(0) ;
	}
	else if (State == 1)
	{
		Count0 ++;
		if (Count0 >= 40)
		{
			Count0 = 0;


			Angle2 = Angle1;
			Angle1 = Angle0;
			Angle0 = Angle;

			if (Angle0 > CENTER_ANGLE + CENTER_RANGE
			 && Angle1 > CENTER_ANGLE + CENTER_RANGE
			 && Angle2 > CENTER_ANGLE + CENTER_RANGE
			 && Angle1 < Angle0
			 && Angle1 < Angle2)
			{
				State = 21;
			}
			if (Angle0 < CENTER_ANGLE - CENTER_RANGE
			 && Angle1 < CENTER_ANGLE - CENTER_RANGE
			 && Angle2 < CENTER_ANGLE - CENTER_RANGE
			 && Angle1 > Angle0
			 && Angle1 > Angle2)
			{
				State = 31;
			}
			if (Angle0 > CENTER_ANGLE - CENTER_RANGE
			 && Angle0 < CENTER_ANGLE + CENTER_RANGE
			 && Angle1 > CENTER_ANGLE - CENTER_RANGE
			 && Angle1 < CENTER_ANGLE + CENTER_RANGE)
			{
				PID_Param_Reset(&PID_AD) ;
				PID_Param_Reset(&Motor.PID_Angle) ;
				PID_AD.goalPoint = CENTER_ANGLE ;
				Motor.PID_Angle.goalPoint = Motor_Angle_Update(&Motor) ;
				State = 4;
			}
		}
	}
	else if (State == 21)
	{
		Motor_SetPWM(START_PWM);
		CountTime = START_TIME;
		State = 22;
	}
	else if (State == 22)
	{
		CountTime --;
		if (CountTime == 0)
		{
			State = 23;
		}
	}
	else if (State == 23)
	{
		Motor_SetPWM(-START_PWM);
		CountTime = START_TIME;
		State = 24;
	}
	else if (State == 24)
	{
		CountTime --;
		if (CountTime == 0)
		{
			Motor_SetPWM(0);
			State = 1;
		}
	}
	else if (State == 31)
	{
		Motor_SetPWM(-START_PWM);
		CountTime = START_TIME;
		State = 32;
	}
	else if (State == 32)
	{
		CountTime --;
		if (CountTime == 0)
		{
			State = 33;
		}
	}
	else if (State == 33)
	{
		Motor_SetPWM(START_PWM);
		CountTime = START_TIME;
		State = 34;
	}
	else if (State == 34)
	{
		CountTime --;
		if (CountTime == 0)
		{
			Motor_SetPWM(0);
			State = 1;
		}
	}
	else if (State == 4)
	{
		if (!(Angle > CENTER_ANGLE - CENTER_RANGE && Angle < CENTER_ANGLE + CENTER_RANGE))
		{
			State = 0 ;
		}
	}
}

void Mode5_5ms_Tick(void)
{
	if (State == 4)
	{
		Timer_Counter_Func() ;
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
	
	if (State == 4)
	{
		// 外环PID: 倒立摆位置环
		// 1. 计算角度
		// 2. 计算PID
		PID_Update(&Motor.PID_Angle ,Motor_Angle_Update(&Motor)) ;
		// 3. 输出角度目标
		PID_AD.goalPoint = CENTER_ANGLE - Motor.PID_Angle.setPoint ;
	}
}

void Mode_5_Exit(void)
{
	State = 0 ;
	Motor_SetPWM(0) ;
}
