#include "Mode_G.h"
#include "AllHeader.h"

Mode_Typedef curr_mode = Mode_Null;
Mode_Typedef next_mode = Mode_1;

void Mode_G_Setup(void)
{
    Initial_ALL();
    Initial_Timer();
}

void Mode_G_Loop(void)
{		
	// 切换模式
	if (Key_Check(KEY_1, KEY_DOUBLE))
	{
			Mode_To_Next();
	}
	// Null提示
	if (curr_mode == Mode_Null)
	{
		OLED_Printf(0,0,OLED_8X16,"===Mode_Null===") ;
	}
}

// Mode定时器
// 1. 1ms定时器
void Timer_1ms_Callback(void)
{
    Key_Tick();
}

// 2. 10ms定时器
void Timer_10ms_Callback(void)
{
	
}

// 3. 20ms定时器
void Timer_20ms_Callback(void)
{
    switch (curr_mode)
    {
        case Mode_Null: break;
        case Mode_1: Mode_1_Tick(); break;
        case Mode_2: Mode_2_Tick(); break;
        case Mode_3: Mode_3_Tick(); break;
        case Mode_4: Mode_4_Tick(); break;
        case Mode_5: Mode_5_Tick(); break;
        case Mode_6: Mode_6_Tick(); break;
        case Mode_7: Mode_7_Tick(); break;
        case Mode_8: Mode_8_Tick(); break;
        case Mode_9: Mode_9_Tick(); break;
        case Mode_10: Mode_10_Tick(); break;
        default: break;
    }
}

// 模式转移函数
void Mode_To_Next(void)
{
    uint32_t next_value = (uint32_t)next_mode + 1U;

    next_mode = (next_value >= (uint32_t)Mode_End)
              ? Mode_Null
              : (Mode_Typedef)next_value;
}

void Mode_ChangeTo(Mode_Typedef nextmode)
{
    if ((uint32_t)nextmode >= (uint32_t)Mode_End)
    {
        return;
    }

    next_mode = nextmode;
}
