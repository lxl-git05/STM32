#include "Mode_G.h"
#include "AllHeader.h"

extern Pid_Typedef PID_AD ;

Mode_Typedef curr_mode = Mode_Null;
Mode_Typedef next_mode = Mode_5;

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
// 2. 5ms定时器
void Timer_5ms_Callback(void)
{
    switch (curr_mode)
    {
        case Mode_Null: break;
        case Mode_1: Mode1_5ms_Tick(); break;
        case Mode_2: Mode2_5ms_Tick(); break;
        case Mode_3: Mode3_5ms_Tick(); break;
        case Mode_4: Mode4_5ms_Tick(); break;
        case Mode_5: Mode5_5ms_Tick(); break;
        case Mode_6: Mode6_5ms_Tick(); break;
        case Mode_7: Mode7_5ms_Tick(); break;
        case Mode_8: Mode8_5ms_Tick(); break;
        case Mode_9: Mode9_5ms_Tick(); break;
        case Mode_10: Mode10_5ms_Tick(); break;
        default: break;
    }
}

// 3. 20ms定时器
void Timer_20ms_Callback(void)
{
    switch (curr_mode)
    {
        case Mode_Null: break;
        case Mode_1: Mode1_20ms_Tick(); break;
        case Mode_2: Mode2_20ms_Tick(); break;
        case Mode_3: Mode3_20ms_Tick(); break;
        case Mode_4: Mode4_20ms_Tick(); break;
        case Mode_5: Mode5_20ms_Tick(); break;
        case Mode_6: Mode6_20ms_Tick(); break;
        case Mode_7: Mode7_20ms_Tick(); break;
        case Mode_8: Mode8_20ms_Tick(); break;
        case Mode_9: Mode9_20ms_Tick(); break;
        case Mode_10: Mode10_20ms_Tick(); break;
        default: break;
    }
}

int Counter = 50 ;		// 50ms基准
int Counter_Nei = 5 ;	// 5ms基准

// 1. 1ms定时器
void Timer_1ms_Callback(void)
{
	// 1分频
  Key_Tick();
	switch (curr_mode)
	{
			case Mode_Null: break;
			case Mode_1: Mode1_1ms_Tick(); break;
			case Mode_2: Mode2_1ms_Tick(); break;
			case Mode_3: Mode3_1ms_Tick(); break;
			case Mode_4: Mode4_1ms_Tick(); break;
			case Mode_5: Mode5_1ms_Tick(); break;
			case Mode_6: Mode6_1ms_Tick(); break;
			case Mode_7: Mode7_1ms_Tick(); break;
			case Mode_8: Mode8_1ms_Tick(); break;
			case Mode_9: Mode9_1ms_Tick(); break;
			case Mode_10: Mode10_1ms_Tick(); break;
			default: break;
	}
	// 5分频
	static int count5 = 0 ;
	if (++count5 >= Counter_Nei)
	{
		PID_AD.dt_s = Counter_Nei * 1.0 / 1000 ;
		count5 = 0 ;
		Timer_5ms_Callback() ;
	}
	// 50分频
	static int count20 = 0 ;
	if (++count20 >= Counter)
	{
		Motor.PID_Angle.dt_s = Counter * 1.0 / 1000 ;
		count20 = 0 ;
		Timer_20ms_Callback() ;
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
