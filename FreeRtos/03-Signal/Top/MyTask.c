#include "MyTask.h"
extern osSemaphoreId_t myBinarySem01Handle;			// 二值信号量
extern osSemaphoreId_t myCountingSem01Handle;		// 计数信号量
// ========================= 全局变量 =========================
bool LED_Flag = true ;


// ===================== 外设初始化 setup ======================
void Initial_ALL(void)
{
	Timer_Counter_Init() ;
	OLED_Init() ;
	Serial_Init() ;
	// 开启TIM2定时器中断
	HAL_TIM_Base_Start_IT(&htim2);
}	

// ========================= Task任务0 =========================
void MyTask_default(void)
{
	static int num = 0 ; 
	OLED_ShowNum(0 , 0  , num++ , 4 , OLED_8X16 ) ;
	OLED_Update() ;
	osDelay(1000) ;	// 阻塞态
}

// ========================= Task任务1 =========================
void MyTask_01(void)
{
	
}

// ========================= Task任务2 =========================
void MyTask_02(void)
{
	
}

// ========================= Task任务3 =========================
void MyTask_03(void)
{
	Key_Tick() ;
	osDelay(1) ;
}

// 使用printf
int fputc(int ch , FILE *f)
{
	while((huart1.Instance->SR & USART_SR_TXE) == 0)
	{
		 ;
	}
	
	huart1.Instance->DR = *(uint8_t*)&ch ;
	
	return ch ;
}

// TIM2定时器
void tim2_cb(TIM_HandleTypeDef *htim)
{
  // 1ms中断
	if (htim->Instance == TIM2) 
	{
		static int cnt = 0 ;
		cnt ++ ;
		if (cnt == 1000)
		{
			cnt = 0 ;
			HAL_GPIO_TogglePin(LED0_GPIO_Port , LED0_Pin ) ;
			
		}
  }
}
