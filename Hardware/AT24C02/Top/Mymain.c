#include "Mymain.h"
#include "Initial.h"
#include "Param_AT24C02.h"

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
    __enable_irq();

    // ---------- AT参数初始化（注册+上电恢复）----------
    Param_AT24C02_Init();

    while (1)
    {
        // ---------- OLED 主界面 ----------
        OLED_Clear();
        OLED_Printf(0,  0, OLED_8X16, "AT24C02 Test");
        OLED_Printf(0, 16, OLED_6X8,  "Mode:%d Spd:%ld",  g_mode,      g_motorSpeed);
        OLED_Printf(0, 26, OLED_6X8,  "Kp:%.2f Ki:%.2f",  g_pidKp,    g_pidKi);
        OLED_Printf(0, 36, OLED_6X8,  "Kd:%.2f",           g_pidKd);
        OLED_Printf(0, 48, OLED_6X8,  "K1:+Spd K2:+Kp");
        OLED_Printf(0, 56, OLED_6X8,  "K1H:Save K2H:+Ki");
        OLED_Update();

        // ---------- KEY1 单击：g_motorSpeed递增10并写入AT ----------
        if (Key_Check(KEY_1, KEY_SINGLE))
        {
            g_motorSpeed += 10;
            if (Param_AT24C02_Write(&g_motorSpeed))
            {
                LED_Flash_Mode_Set_Mode(LED_Flash_Fast);
                s_ledTimer = 500;
            }
        }

        // ---------- KEY2 单击：g_pidKp递增0.1并写入AT ----------
        if (Key_Check(KEY_2, KEY_SINGLE))
        {
            g_pidKp += 0.1f;
            if (Param_AT24C02_Write(&g_pidKp))
            {
                LED_Flash_Mode_Set_Mode(LED_Flash_Fast);
                s_ledTimer = 500;
            }
        }

        // ---------- KEY1 长按：保存所有参数到AT ----------
        if (Key_Check(KEY_1, KEY_LONG))
        {
            Param_AT24C02_SaveAll();
            LED_Flash_Mode_Set_Mode(LED_Flash_Fast);
            s_ledTimer = 1000;

            OLED_Clear();
            OLED_Printf(0, 0, OLED_8X16, "All Saved!");
            OLED_Printf(0, 20, OLED_6X8, "Mode:%d Spd:%ld", g_mode, g_motorSpeed);
            OLED_Printf(0, 30, OLED_6X8, "Kp:%.2f Ki:%.2f", g_pidKp, g_pidKi);
            OLED_Printf(0, 40, OLED_6X8, "Kd:%.2f", g_pidKd);
            OLED_Update();
            HAL_Delay(1500);
        }

        // ---------- KEY2 长按：g_pidKi递增0.1并写入AT ----------
        if (Key_Check(KEY_2, KEY_LONG))
        {
            g_pidKi += 0.1f;
            if (Param_AT24C02_Write(&g_pidKi))
            {
                LED_Flash_Mode_Set_Mode(LED_Flash_Fast);
                s_ledTimer = 500;
            }
        }

        // ---------- KEY1 双击：读取AT所有参数（覆盖本地） ----------
        if (Key_Check(KEY_1, KEY_DOUBLE))
        {
            Param_AT24C02_SaveAll();
            OLED_Clear();
            OLED_Printf(0, 0, OLED_8X16, "All Loaded!");
            OLED_Printf(0, 20, OLED_6X8, "Mode:%d Spd:%ld", g_mode, g_motorSpeed);
            OLED_Printf(0, 30, OLED_6X8, "Kp:%.2f Ki:%.2f", g_pidKp, g_pidKi);
            OLED_Printf(0, 40, OLED_6X8, "Kd:%.2f", g_pidKd);
            OLED_Update();
            HAL_Delay(1500);
        }

        HAL_Delay(50);
    }
}

// Systick定时中断,1ms触发
void HAL_SYSTICK_Callback(void)
{
    Key_Tick();
    LED_Flash_Mode_Tick();

    if (s_ledTimer > 0)
    {
        if (--s_ledTimer == 0)
            LED_Flash_Mode_Set_Mode(LED_Flash_Slow);
    }
}
