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

// 旋转编码器测试变量
int16_t Num = 0;

// =================== 主函数 ===================
void Mymain(void)
{
    Initial_ALL();
    // Param_AT24C02_Init();  // 暂不启用
    __enable_irq();

    Encoder_Init();
    OLED_ShowString(0, 0, "Num:", OLED_8X16);
    

    while (1)
    {
        Num += Encoder_Get();
        OLED_ShowSignedNum(0, 20, (int32_t)Num, 5, OLED_8X16);
				OLED_Update() ;
    }
}

// Systick定时中断,1ms触发
void HAL_SYSTICK_Callback(void)
{
    Key_Tick();
    LED_Flash_Mode_Tick();
}
