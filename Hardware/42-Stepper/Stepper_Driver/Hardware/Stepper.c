#include "Stepper.h"

Stepper_Typedef Stepper1;	// 1号步进电机
Stepper_Typedef Stepper2;	// 2号步进电机

#define Stepper1_huart huart3
#define Stepper2_huart huart6
#define Stepper1_hdma_usart_rx hdma_usart3_rx
#define Stepper2_hdma_usart_rx hdma_usart6_rx

extern DMA_HandleTypeDef Stepper1_hdma_usart_rx;
extern DMA_HandleTypeDef Stepper2_hdma_usart_rx;

// 1. 初始化
void Stepper_Init(void)
{
	// 关闭DMA的缓存
	SCB_DisableDCache();	// 一定要记得这个，否则缓存更新会使得数组数据不更新

	// 初始化步进电机
	Stepper1.Stepper_huart = &Stepper1_huart;
	Stepper1.Stepper_dma   = &Stepper1_hdma_usart_rx;
	Stepper1.addr = 0x1;
	Stepper1.Positive_Dir = 1;
	Stepper1.pulse_angle = 0.004f;  // 1.8f / (75.0f / 27.0f) / 162
	Stepper1.tx_busy = false;

	Stepper2.Stepper_huart = &Stepper2_huart;
	Stepper2.Stepper_dma   = &Stepper2_hdma_usart_rx;
	Stepper2.addr = 0x1;	// Stepper都是地址1
	Stepper2.Positive_Dir = 1;
	Stepper2.pulse_angle = 0.01f;  // 1.8f / 180
	Stepper2.tx_busy = false;

	// RX_DMA
	HAL_UARTEx_ReceiveToIdle_DMA(&Stepper1_huart, (uint8_t *)Stepper1.rx_buf, CMD_LEN);
	HAL_UARTEx_ReceiveToIdle_DMA(&Stepper2_huart, (uint8_t *)Stepper2.rx_buf, CMD_LEN);

	// 等待初始化完成
	HAL_Delay(500);
}

// ======================== 配置相关参数 ========================

// 2. 配置目标速度和加速度
void Stepper_Speed_Set(Stepper_Typedef* pStepper, int16_t vel, uint8_t acc)
{
	while (pStepper->tx_busy);  // 等待TX完成
	pStepper->tx_busy = true;   // 标记TX忙

	if (vel * pStepper->Positive_Dir > 0)
	{
		Emm_V5_Vel_Control(pStepper->Stepper_huart, pStepper->addr, 0, vel, acc, false);
	}
	else
	{
		Emm_V5_Vel_Control(pStepper->Stepper_huart, pStepper->addr, 1, -vel, acc, false);
	}
}

// 3. 配置目标旋转角度 - 相对当前电机位置运动
void Stepper_Relative_Pos_Set(Stepper_Typedef* pStepper, int16_t vel, uint8_t acc, float angle)
{
	while (pStepper->tx_busy);  // 等待TX完成
	pStepper->tx_busy = true;   // 标记TX忙

	uint32_t clk = (uint32_t)(angle / pStepper->pulse_angle);
	uint8_t dir = (vel * pStepper->Positive_Dir > 0) ? 0 : 1;
	vel = abs(vel);
	Emm_V5_Pos_Control(pStepper->Stepper_huart, pStepper->addr, dir, vel, acc, clk, 2, false);
}

// 4. 配置目标角度 - 绝对位置运动
void Stepper_Absolute_Pos_Set(Stepper_Typedef* pStepper, int16_t vel, uint8_t acc, float angle)
{
	while (pStepper->tx_busy);  // 等待TX完成
	pStepper->tx_busy = true;   // 标记TX忙

	uint32_t clk = (uint32_t)(angle / pStepper->pulse_angle);
	uint8_t dir = (vel * pStepper->Positive_Dir > 0) ? 0 : 1;
	vel = abs(vel);
	Emm_V5_Pos_Control(pStepper->Stepper_huart, pStepper->addr, dir, vel, acc, clk, 1, false);
}

// 5. 配置当前位置为0点
void Stepper_Reset_Zero(Stepper_Typedef* pStepper)
{
	while (pStepper->tx_busy);  // 等待TX完成
	pStepper->tx_busy = true;   // 标记TX忙

	Emm_V5_Reset_CurPos_To_Zero(pStepper->Stepper_huart, pStepper->addr);
}

// ======================== 得到相关参数(一般放在中断查询) ========================

// 1. 得到当前角度
void Stepper_Pos_Update(Stepper_Typedef* pStepper)
{
	if (!pStepper->tx_busy)
	{
		pStepper->tx_busy = true ;
		Emm_V5_Read_Sys_Params(pStepper->Stepper_huart, pStepper->addr, S_CPOS);
	}
}

// 2. 得到当前速度
void Stepper_Vel_Update(Stepper_Typedef* pStepper)
{
	if (!pStepper->tx_busy)  // 只检查本电机的 TX 状态
	{
		pStepper->tx_busy = true ;
		Emm_V5_Read_Sys_Params(pStepper->Stepper_huart, pStepper->addr, S_VEL);
	}
}

// 3. 得到目标角度
void Stepper_Target_Pos_Update(Stepper_Typedef* pStepper)
{
	if (!pStepper->tx_busy)  // 只检查本电机的 TX 状态
	{
		pStepper->tx_busy = true ;
		Emm_V5_Read_Sys_Params(pStepper->Stepper_huart, pStepper->addr, S_TPOS);
	}
}

// ======================== 相应回复信息 ========================

// 1. 实时速度接收处理
void Stepper_Speed_Deal(Stepper_Typedef* pStepper)
{
	if(pStepper->rx_buf[0] == pStepper->addr && pStepper->rx_buf[1] == 0x35 && pStepper->rxCount == 6)
	{
		// 拼接成uint16_t类型数据
		pStepper->Speed_Now = (uint16_t)(
											((uint16_t)pStepper->rx_buf[3] << 8)   |
											((uint16_t)pStepper->rx_buf[4] << 0)
										);

		// 符号
		if(pStepper->rx_buf[2]) { pStepper->Speed_Now = -pStepper->Speed_Now; }
	}
}

// 2. 实时角度接收处理
void Stepper_Angle_Deal(Stepper_Typedef* pStepper)
{
	if(pStepper->rx_buf[0] == pStepper->addr && pStepper->rx_buf[1] == 0x36 && pStepper->rxCount == 8)
	{
		// 拼接成uint32_t类型
		float pos = (uint32_t)(
											((uint32_t)pStepper->rx_buf[3] << 24)    |
											((uint32_t)pStepper->rx_buf[4] << 16)    |
											((uint32_t)pStepper->rx_buf[5] << 8)     |
											((uint32_t)pStepper->rx_buf[6] << 0)
										);

		// 转换成角度
		pStepper->Pos_Now = (float)pos * 360.0f / 65536.0f;

		// 符号
		if(pStepper->rx_buf[2]) { pStepper->Pos_Now = -pStepper->Pos_Now; }
	}
}

// 串口RX_DMA处理,放在stm32h7xx_it.c的相应串口位置
void Stepper_Rx_DMA_Cplt(Stepper_Typedef* pStepper)
{
	if(__HAL_UART_GET_FLAG(pStepper->Stepper_huart, UART_FLAG_IDLE) != RESET)
	{
		__HAL_UART_CLEAR_IDLEFLAG(pStepper->Stepper_huart); // 清除IDLE标志

		HAL_UART_DMAStop(pStepper->Stepper_huart); // 停止DMA，为了重新设置DMA发送多少数据
		
		// 数据处理
		pStepper->rxCount = CMD_LEN - __HAL_DMA_GET_COUNTER(pStepper->Stepper_dma);
		 
		pStepper->rxFlag = true; // 置位一帧命令接收完毕标志位

		// 接收数据特殊处理
		// 1. 实时速度接收处理
		Stepper_Speed_Deal(pStepper) ;
		// 2. 实时角度接收处理
		Stepper_Angle_Deal(pStepper) ;

		// 重启
		HAL_UARTEx_ReceiveToIdle_DMA(pStepper->Stepper_huart, (uint8_t *)pStepper->rx_buf, CMD_LEN);
	}
}


