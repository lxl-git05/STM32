#include "Initial.h"

// Mymain��ʼ������,ʹ�����������
void Initial_ALL(void)
{
	// System
	HAL_SYSTICK_Config(SystemCoreClock / 1000);		// Systick��ʼ��
	
	// Hardware
	OLED_Init() ;																	// OLED��ʼ��
	
	// Software
	Serial_Init() ;																// ���ڳ�ʼ��
	
	// Tools
	LED_Flash_Mode_Init() ;												// LED��˸���߳�ʼ��
	Timer_Counter_Init() ;												// ʱ����ⶨ��ʼ��

	// Top
	// Menu_Init() ;   // 已废弃，改用 ParamEdit 模块
}
