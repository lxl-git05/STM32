#ifndef __28BYJ48_H
#define __28BYJ48_H

#include "main.h"

// 配置宏
#define BYJ_MAX_RPM         11  		// 最大RPM
#define BYJ_MAX_Rotate_RPM   9   		// 最大RPM
#define BYJ_htim   		 			&htim2  // 28BYJ驱动定时器句柄
#define BYJ_TIM					    TIM2    // 共享定时器实例
#define BYJ_STEPS_PER_REV   4096    // 一圈步数（半步模式）

// 方向枚举
typedef enum
{
    BYJ_Shun,  // 顺时针
    BYJ_Ni     // 逆时针
} BYJ_Pos_Typedef;

// 每个电机实例的配置结构体（GPIO端口/引脚需用户传入）
typedef struct
{
	GPIO_TypeDef* GPIO_Port[4];  // IN1-IN4 端口数组
	uint16_t GPIO_Pin[4];        // IN1-IN4 引脚数组
	
	uint8_t Enabled;             // 启用标志（1:运行，0:停止）
	int8_t Step;                 // 当前步数（0-7）

	BYJ_Pos_Typedef Pos;         // 方向,默认顺时针
	int RPM;                     // 当前RPM，默认0
	
	int Current_Step ;					 // 当前角度,默认为0
	int Target_Step  ;					 // 目标角度,默认为0,当目标角度不为0时角度环优先
	
} BYJ_Typedef;

// =================== 函数 ===================
// 初始化BYJ
HAL_StatusTypeDef BYJ_Init( BYJ_Typedef * BYJ,
							 GPIO_TypeDef* port1, uint16_t pin1,
							 GPIO_TypeDef* port2, uint16_t pin2,
							 GPIO_TypeDef* port3, uint16_t pin3,
							 GPIO_TypeDef* port4, uint16_t pin4) ;
// 配置方向
void BYJ_Set_Position(BYJ_Typedef* BYJ ,BYJ_Pos_Typedef Pos);
// 配置速度
void BYJ_Set_RPM(BYJ_Typedef* BYJ , int RPM);
// 停止
void BYJ_Stop(BYJ_Typedef* BYJ) ;
// 在中断驱动BYJ
void BYJ_Drive(BYJ_Typedef* BYJ) ;
// 配置目标角度
void BYJ_Set_Target_Angle(BYJ_Typedef* BYJ , int RPM , int Angle) ;
#endif
