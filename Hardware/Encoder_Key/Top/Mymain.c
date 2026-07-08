#include "Mymain.h"
#include "Initial.h"

#include "Encoder_Key.h"

// =================== 本地测试状态 ===================
static uint16_t s_ledTimer = 0;
int32_t  g_motorSpeed = 0;      // 电机速度
float    g_pidKp      = 1.0f;  // PID比例系数
float    g_pidKi      = 0.0f;  // PID积分系数
float    g_pidKd      = 0.0f;  // PID微分系数
int8_t   g_mode       = 0;     // 运行模式（占位，用户自行扩展）

// =================== 主函数 ===================
void Mymain(void)
{
    Initial_ALL();
		Param_AT24C02_Init();
    __enable_irq();
    

    while (1)
    {
			// 旋转编码器测试
			
        
    }
}

// Systick定时中断,1ms触发
void HAL_SYSTICK_Callback(void)
{
    Key_Tick();
    LED_Flash_Mode_Tick();
}
