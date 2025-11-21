#include "Mymain.h"

void Mymain(void)
{
	// ******************* setup *******************
	// ???
	{
		HAL_SYSTICK_Config(SystemCoreClock / 1000);
		OLED_Init() ;
		Serial_Init(&Serial_huart) ;
		// ***???????????Systick??,????sys???????????***
		__enable_irq();	
	}
	// *******************????*******************
//	// Debug????
//	int check1 ;
//	int check2 ;
//	int check[50] ;
	while(1)
	{
		// ******************* while *******************
		// ******************* ???? *******************
		
		// ????:OLED??
		OLED_Update() ;
	}
}

// Systick????,1ms??
void HAL_SYSTICK_Callback(void)
{
	// ??
	static int count_sys = 0 ;
	count_sys ++ ;
	// ??1: ??
	Key_Tick() ;
	// ??2:
	if (count_sys % 1000 == 0)
	{
//		HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin ) ;
	}
}
