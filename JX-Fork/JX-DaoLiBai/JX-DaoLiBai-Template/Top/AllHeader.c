#include "AllHeader.h"

void Initial_ALL(void)
{
	OLED_Init() ;
	Serial_Init() ;
}

void Initial_Timer(void)
{
	Timer_Initial() ;
}
