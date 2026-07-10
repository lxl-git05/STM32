#ifndef __STEPPER_H
#define __STEPPER_H

#include "MySystem.h"
#include "Emm_V5.h"

// 串口驱动步进电机

// 接收数据的宏定义
#define		CMD_LEN		255

// 步进电机结构体
typedef struct
{
	// 通信
	UART_HandleTypeDef *Stepper_huart ;	// 串口驱动口
	DMA_HandleTypeDef  *Stepper_dma		;	// DMA驱动口
	uint8_t addr ;											// 地址
	uint8_t rxCount ;										// 读取的数据数
	bool rxFlag ;												// 1为接收
	uint8_t rx_buf[CMD_LEN];            // 接收缓冲(可按需调大)
	
	// 电机参数
	float pulse_angle;  								// 单位脉冲角度(度),需要根据 Type(0.9 or 1.8),Ratio(齿轮比),细分 三个来确定
	int8_t Positive_Dir ;								// 正方向(1 or -1)
	
	// 速度
	int Speed_Tar ;											// 目标速度
	int Speed_Now ;											// 当前速度
	
	// 角度
	float Pos_Tar ;											// 目标角度
	float Pos_Now ;											// 当前角度
	
}Stepper_Typedef ;

extern Stepper_Typedef Stepper1 ;	// 1号步进电机
extern Stepper_Typedef Stepper2 ;	// 2号步进电机

// ======================== 配置相关参数 ========================
// 1. 初始化
void Stepper_Init(void) ;
// 2. 配置目标速度和加速度
void Stepper_Speed_Set(Stepper_Typedef* pStepper, int16_t vel, uint8_t acc);
// 3. 配置目标旋转角度(相对位置)
void Stepper_Relative_Pos_Set(Stepper_Typedef* pStepper, int16_t vel, uint8_t acc, float angle);
// 4. 配置目标角度(绝对位置)
void Stepper_Absolute_Pos_Set(Stepper_Typedef* pStepper, int16_t vel, uint8_t acc, float angle);
// 5. 配置当前位置为0点
void Stepper_Reset_Zero(Stepper_Typedef* pStepper);

// ======================== 得到相关参数 ========================
// 1. 得到当前角度
void Stepper_Pos_Update(Stepper_Typedef* pStepper);
// 2. 得到当前速度
void Stepper_Vel_Update(Stepper_Typedef* pStepper);
// 3. 得到目标角度
void Stepper_Target_Pos_Update(Stepper_Typedef* pStepper);

// ======================== 串口相关 ========================
// 串口RX_DMA处理,放在stm32h7xx_it.c的相应串口位置
void Stepper_Rx_DMA_Cplt(Stepper_Typedef* pStepper) ;

// 外部变量声明(在Stepper.c中定义)
extern float Motor_Vel;          // 当前速度
extern float Motor_Cur_Pos;      // 当前角度

#endif
