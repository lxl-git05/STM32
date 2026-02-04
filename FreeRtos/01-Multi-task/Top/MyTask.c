#include "MyTask.h"
// ========================= 全局变量 =========================
bool LED_Flag = true ;


// ===================== 外设初始化 setup ======================
void Initial_ALL(void)
{
	Timer_Counter_Init() ;
	OLED_Init() ;
	Serial_Init() ;
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
	printf("A") ;
	portYIELD() ;	// 在1个时间片 内 执行完后立即切换到下一个任务
}

// ========================= Task任务2 =========================
void MyTask_02(void)
{
	printf("123456789abcdefgh") ;
}

// ========================= Task任务3 =========================
void MyTask_03(void)
{
	printf("C") ;
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
