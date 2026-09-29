#include "AllHeader.h"
#include <limits.h>

static int num = 1;

void Mode_2_Setup(void) {}

void Mode_2_Loop(void)
{
    OLED_Printf(0, 0, OLED_6X8, "===Mode_2===");
    if (Key_Check(KEY_1, KEY_SINGLE) && num <= INT_MAX / 10) num *= 10;
    if (Key_Check(KEY_1, KEY_DOUBLE)) num /= 10;
    if (Key_Check(KEY_1, KEY_LONG)) num = 0;
    OLED_Printf(0, 16, OLED_8X16, "%d", num);
}

void Mode_2_Tick(void)
{
    Serial_printf(&Serial1, "%d\n", num);
}

void Mode_2_Exit(void) {}
