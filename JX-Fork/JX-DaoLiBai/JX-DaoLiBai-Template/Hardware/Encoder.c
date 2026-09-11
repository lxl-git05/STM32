#include "Encoder.h"
#include "tim.h"

void Encoder_Init(void)
{
	// 启动编码器接口
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_ALL);
}

int16_t Encoder_Get(void)
{
	int16_t Temp;
	Temp = __HAL_TIM_GET_COUNTER(&htim3);
	__HAL_TIM_SET_COUNTER(&htim3 , 0) ;
	return -Temp;	// 返回反向
}
