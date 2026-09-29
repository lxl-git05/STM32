#include "Mode_G.h"
#include "AllHeader.h"
Mode_Typedef curr_mode = Mode_Null;
Mode_Typedef next_mode = Mode_1	  ;
void Mode_G_Setup(void)
{
    Initial_ALL();
    Initial_Timer();
}
void Mode_G_Loop(void)
{
    if (Key_Check(KEY_0, KEY_SINGLE)) Mode_To_Next();
    if (curr_mode == Mode_Null)
        OLED_Printf(0, 0, OLED_6X8, "===Mode_G===");
}
void Mode_To_Next(void)
{
    uint32_t value = (uint32_t)next_mode + 1U;
    next_mode = value >= (uint32_t)Mode_End ? Mode_Null : (Mode_Typedef)value;
}
void Mode_ChangeTo(Mode_Typedef mode)
{
    if (mode < Mode_End) next_mode = mode;
}
