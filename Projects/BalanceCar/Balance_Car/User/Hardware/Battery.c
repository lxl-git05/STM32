#include "Battery.h"
#include "adc.h"

#define BATTERY_EMPTY_MV 9000U
#define BATTERY_FULL_MV  12400U

void Battery_Init(void)
{
    ADC_ChannelConfTypeDef channel = {0};
    channel.Channel = ADC_CHANNEL_9;
    channel.Rank = ADC_REGULAR_RANK_1;
    channel.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    if (HAL_ADC_ConfigChannel(&hadc1, &channel) != HAL_OK ||
        HAL_ADCEx_Calibration_Start(&hadc1) != HAL_OK)
    {
        Error_Handler();
    }
}

int Battery_GetValue(void)
{
    int percent = -1;
    if (HAL_ADC_Start(&hadc1) == HAL_OK && HAL_ADC_PollForConversion(&hadc1, 10U) == HAL_OK)
    {
        // PB1: 120k/33k divider, 3.3V ADC reference; 3S voltage-based estimate.
        uint32_t mv = HAL_ADC_GetValue(&hadc1) * 3300U * 153U / (4095U * 33U);
        percent = mv <= BATTERY_EMPTY_MV ? 0 : mv >= BATTERY_FULL_MV ? 100 :
                  (int)((mv - BATTERY_EMPTY_MV) * 100U / (BATTERY_FULL_MV - BATTERY_EMPTY_MV));
    }
    HAL_ADC_Stop(&hadc1);
    return percent;
}
