#include "Timer.h"

// 弱定义
// 定时器初始化
void Timer_Initial(void)
{
	// 1ms定时器
	HAL_SYSTICK_Config(SystemCoreClock / 1000); //0.001s定时周期(SystemCoreClock / x)
	// 20ms定时器
//	HAL_TIM_Base_Start_IT(&htim6);
}

/*弱函数声明,主函数调用*/
__attribute__((weak)) void Timer_1ms_Callback(void)
{
    // 默认空实现
}

/*弱函数声明,主函数调用*/
__attribute__((weak)) void Timer_10ms_Callback(void)
{
    // 默认空实现
}

/*弱函数声明,主函数调用*/
__attribute__((weak)) void Timer_20ms_Callback(void)
{
    // 默认空实现
}

//// 定时器回调函数（1ms已经在fxx.it.c内置了）
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
////  //判断是否是TIM6中断
////  if (htim->Instance == TIM6)
////  {
////    Timer_1ms_Callback() ;	// 1ms中断
////  }
////	// 判断是否是TIM7中断
////	if (htim->Instance == TIM7)
////	{
////		Timer_20ms_Callback() ;	// 20ms中断
////	}
////	// 判断是否是TIM10中断（球平衡 10ms）
////	if (htim->Instance == TIM10)
////	{
////		Timer_10ms_Callback() ;	// 10ms中断
////	}
//}

// ========== 全局中断开关 ==========
// 标定/Flash写入等场景需要独占I2C等资源，防止ISR抢占
void Timer_DisableIRQ(void)
{
    __disable_irq();
}

void Timer_EnableIRQ(void)
{
    __enable_irq();
}
