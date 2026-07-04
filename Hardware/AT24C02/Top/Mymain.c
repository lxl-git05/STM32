#include "Mymain.h"
#include "Initial.h"
#include "bsp_at24c02.h"

// =================== 全局变量 ===================
static uint8_t s_writeCnt = 0;       // 已写入次数
static uint16_t s_ledTimer = 0;      // LED快闪持续计时(ms)

// =================== 实例代码 ===================
void Mymain(void)
{
    Initial_ALL();
    __enable_irq();

    // AT24C02 初始化（硬件I2C2已在MX_I2C2_Init中初始化）
    AT24C02_Init();

    while (1)
    {
        // ---------- OLED 主界面 ----------
        OLED_Clear();
        OLED_Printf(0,  0, OLED_8X16, "AT24C02 Test");
        OLED_Printf(0, 20, OLED_6X8,  "Cnt:%3u", s_writeCnt);
        OLED_Printf(0, 30, OLED_6X8,  "Val:%3u", AT24C02_ReadByte(0));
        OLED_Printf(0, 45, OLED_6X8,  "K1:+1  K2:Read");
        OLED_Update();

        // ---------- KEY1 单击：地址0写入递增计数 ----------
        if (Key_Check(KEY_1, KEY_SINGLE))
        {
            s_writeCnt++;
            if (AT24C02_WriteByte(0, s_writeCnt))
            {
                // 写入成功，LED快闪1秒
                LED_Flash_Mode_Set_Mode(LED_Flash_Fast);
                s_ledTimer = 1000;
            }
        }

        // ---------- KEY2 单击：读取并显示 ----------
        if (Key_Check(KEY_2, KEY_SINGLE))
        {
            uint8_t val = AT24C02_ReadByte(0);
            OLED_Clear();
            OLED_Printf(0, 0, OLED_8X16, "Read Addr0");
            OLED_Printf(0, 20, OLED_6X8, "Val = %u", val);
            OLED_Update();
            HAL_Delay(800);
        }

        HAL_Delay(50);
    }
}

// Systick定时中断,1ms触发
void HAL_SYSTICK_Callback(void)
{
    Key_Tick();
    LED_Flash_Mode_Tick();

    // LED快闪计时，1秒后恢复慢闪
    if (s_ledTimer > 0)
    {
        if (--s_ledTimer == 0)
            LED_Flash_Mode_Set_Mode(LED_Flash_Slow);
    }
}
