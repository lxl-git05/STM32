#include "AllHeader.h"
#include "MyISR.h"

void Initial_ALL(void)
{
    OLED_Init();
    Serial_Init();
}

void Initial_Timer(void)
{
    MyISR_Start();
}
