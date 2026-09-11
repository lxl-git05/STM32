#include "RP.h"

static const uint32_t RP_ChannelTable[4] =
{
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5
};

void RP_Init(void)
{
    /* STM32F1 ADC 上电后校准一次 */
    HAL_ADCEx_Calibration_Start(&hadc2);
}

uint16_t RP_GetValue(uint8_t n)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    uint16_t value;

    /* n 的有效范围为 1～4 */
    if ((n < 1U) || (n > 4U))
    {
        return 0xFFFFU;
    }

    /* 动态选择 ADC2 通道 */
    sConfig.Channel = RP_ChannelTable[n - 1U];
    sConfig.Rank = ADC_REGULAR_RANK_1;

    /*
     * 电位器等阻抗较高的模拟信号不建议使用 1.5 周期，
     * 这里使用 55.5 周期提高采样稳定性。
     */
    sConfig.SamplingTime = ADC_SAMPLETIME_55CYCLES_5;

    if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
    {
        return 0xFFFFU;
    }

    if (HAL_ADC_Start(&hadc2) != HAL_OK)
    {
        return 0xFFFFU;
    }

    if (HAL_ADC_PollForConversion(&hadc2, 10U) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc2);
        return 0xFFFFU;
    }

    value = (uint16_t)HAL_ADC_GetValue(&hadc2);

    HAL_ADC_Stop(&hadc2);

    return value;
}
