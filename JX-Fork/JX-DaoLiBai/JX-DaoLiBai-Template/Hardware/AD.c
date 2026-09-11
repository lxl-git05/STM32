#include "AD.h"

void AD_Init(void)
{
  HAL_ADCEx_Calibration_Start(&hadc1);
}

uint16_t AD_GetValue(void)
{
  uint16_t value;

  if (HAL_ADC_Start(&hadc1) != HAL_OK)
  {
    return AD_ERROR_VALUE;
  }

  if (HAL_ADC_PollForConversion(&hadc1, 10U) != HAL_OK)
  {
    HAL_ADC_Stop(&hadc1);
    return AD_ERROR_VALUE;
  }

  value = (uint16_t)HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);

  return value;
}
