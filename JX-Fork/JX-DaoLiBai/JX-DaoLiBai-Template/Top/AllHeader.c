#include "AllHeader.h"

void Initial_ALL(void)
{
	OLED_Init() ;
	Serial_Init() ;
	RP_Init() ;
	Motor_Init() ;
	Encoder_Init() ;
	AD_Init() ;
}

void Initial_Timer(void)
{
	Timer_Initial() ;
}
