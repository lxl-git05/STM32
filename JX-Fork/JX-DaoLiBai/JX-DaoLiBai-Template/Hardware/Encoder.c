#include "Encoder.h"
#include "tim.h"

int32_t Encoder_cnt = 0 ;

void Encoder_Init(void)
{
	// 启动编码器接口
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = -__HAL_TIM_GET_COUNTER(&htim3);// 返回反向
	Encoder_cnt += Temp ;	// 计算总脉冲
	__HAL_TIM_SET_COUNTER(&htim3 , 0) ;
	return Temp;	
}

int16_t Encoder_cnt_Get(void)
{
	return Encoder_cnt ;
}
