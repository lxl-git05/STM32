#include "28byj48.h"
#include "tim.h"
// =============== 底层代码 ===============

// 初始化BYJ
HAL_StatusTypeDef BYJ_Init( BYJ_Typedef * BYJ,
							 GPIO_TypeDef* port1, uint16_t pin1,
							 GPIO_TypeDef* port2, uint16_t pin2,
							 GPIO_TypeDef* port3, uint16_t pin3,
							 GPIO_TypeDef* port4, uint16_t pin4)
{
	// 启动定时器中断模式
	HAL_TIM_Base_Start_IT(&htim2);
	
	// 配置GPIO
	BYJ->GPIO_Port[0] = port1; BYJ->GPIO_Pin[0] = pin1;
	BYJ->GPIO_Port[1] = port2; BYJ->GPIO_Pin[1] = pin2;
	BYJ->GPIO_Port[2] = port3; BYJ->GPIO_Pin[2] = pin3;
	BYJ->GPIO_Port[3] = port4; BYJ->GPIO_Pin[3] = pin4;
	
	BYJ->Current_Step = 0 ;
	BYJ->Target_Step  = 0 ;
	
	BYJ->Pos = BYJ_Shun;
	BYJ->RPM = 0;
	BYJ->Step = 0;
	BYJ->Enabled = 1;	// 事实上在这里启动/关闭 与 RPM没有区别，但是保留相应功能以待后续开发

	return HAL_OK;
}
// 配置步进电机的节拍
void BYJ_Set_Step(BYJ_Typedef* BYJ ,uint8_t IN1_State ,uint8_t IN2_State ,uint8_t IN3_State ,uint8_t IN4_State )
{
	// 配置节拍
	HAL_GPIO_WritePin(BYJ->GPIO_Port[0], BYJ->GPIO_Pin[0], (GPIO_PinState)IN1_State);
	HAL_GPIO_WritePin(BYJ->GPIO_Port[1], BYJ->GPIO_Pin[1], (GPIO_PinState)IN2_State);
	HAL_GPIO_WritePin(BYJ->GPIO_Port[2], BYJ->GPIO_Pin[2], (GPIO_PinState)IN3_State);
	HAL_GPIO_WritePin(BYJ->GPIO_Port[3], BYJ->GPIO_Pin[3], (GPIO_PinState)IN4_State);
}

// 28BYJ-48 半步驱动（8步序列）
void BYJ_Half_Drive(BYJ_Typedef* BYJ , uint8_t step)
{
	switch(step)
	{
		case 0:  BYJ_Set_Step(BYJ,1,0,0,0); break;
		case 1:  BYJ_Set_Step(BYJ,1,1,0,0); break;
		case 2:  BYJ_Set_Step(BYJ,0,1,0,0); break;
		case 3:  BYJ_Set_Step(BYJ,0,1,1,0); break;
		case 4:  BYJ_Set_Step(BYJ,0,0,1,0); break;
		case 5:  BYJ_Set_Step(BYJ,0,0,1,1); break;
		case 6:  BYJ_Set_Step(BYJ,0,0,0,1); break;
		case 7:  BYJ_Set_Step(BYJ,1,0,0,1); break;
		default: BYJ_Set_Step(BYJ,0,0,0,0); break;
	}
}
// =============== 驱动控制代码 ===============

// 配置方向
void BYJ_Set_Position(BYJ_Typedef* BYJ ,BYJ_Pos_Typedef Pos)
{
	// 默认为顺时针
	BYJ->Pos = Pos ;
}

// 配置速度
void BYJ_Set_RPM(BYJ_Typedef* BYJ , int RPM)
{
	// 限幅
	if (RPM > BYJ_MAX_RPM) RPM = BYJ_MAX_RPM;
	if (RPM < 0) RPM = 0;
	
	// 配置RPM
	BYJ->RPM = RPM;

	if (RPM == 0)
	{
		BYJ_Set_Step(BYJ,0,0,0,0); // 关闭所有相
	}
	else
	{
		// 计算 ARR：计数频率1MHz，60,000,000 / (RPM * 4096) - 1
		uint32_t arr = (60000000UL / (RPM * 4096UL)) - 1;
		BYJ_TIM->ARR = arr;
	}
}
// 停止
void BYJ_Stop(BYJ_Typedef* BYJ)
{
	BYJ_Set_RPM(BYJ , 0) ;
}

// 配置目标角度
void BYJ_Set_Target_Angle(BYJ_Typedef* BYJ , int RPM , int Angle)
{
	int steps = (Angle * BYJ_STEPS_PER_REV) / 360; // 计算步数（整数近似）
	if (RPM >= BYJ_MAX_Rotate_RPM )	// 旋转最大速度,亲测
	{
		RPM = BYJ_MAX_Rotate_RPM;	
	}
	BYJ_Set_RPM(BYJ , RPM) ;											 // 旋转目标角度的速度
	BYJ->Target_Step = steps ;										 // 目标角度,正为顺时针，负为逆时针
}

// =============== 顶层驱动代码 =============== 

// 在中断驱动BYJ
void BYJ_Drive(BYJ_Typedef* BYJ)
{
	if (BYJ->Enabled == 1)
	{	
		if (BYJ->Target_Step != 0) // 角度模式，优先，知道目标为0
		{
			if (BYJ->Target_Step > 0)
			{
				BYJ->Step ++ ;
				BYJ->Current_Step ++ ;
			}
			else
			{
				BYJ->Step -- ;
				BYJ->Current_Step -- ;
			}
			// 判断角度
			if (BYJ->Current_Step == BYJ->Target_Step)
			{
				BYJ_Stop(BYJ) ;
				BYJ->Target_Step  = 0 ;		// 清零，相当重要
				BYJ->Current_Step = 0 ;
			}
			// 运动
			if (BYJ->Step > 7) BYJ->Step = 0;
			if (BYJ->Step < 0) BYJ->Step = 7;
			
			BYJ_Half_Drive(BYJ , BYJ->Step) ;
		}
		else if (BYJ->RPM != 0)			// 速度模式
		{
			if (BYJ->Pos == BYJ_Shun)
			{
				BYJ->Step ++ ;
			}
			else
			{
				BYJ->Step -- ;
			}
			// 运动
			if (BYJ->Step > 7) BYJ->Step = 0;
			if (BYJ->Step < 0) BYJ->Step = 7;
			
			BYJ_Half_Drive(BYJ , BYJ->Step) ;
		}
	}
}
