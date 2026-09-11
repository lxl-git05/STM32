#include "AllHeader.h"

void Initial_ALL(void)
{
    /* Add application module initialization here. */
	OLED_Init() ;
}

void Initial_Timer(void)
{
    /* Timer callbacks are reserved but are not enabled yet. */
	Timer_Initial() ;
}
