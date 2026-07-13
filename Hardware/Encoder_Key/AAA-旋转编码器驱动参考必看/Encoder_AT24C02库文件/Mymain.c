#include "Mymain.h"
#include "Initial.h"
#include "Param_AT24C02.h"
#include "Encoder_Key.h"
#include "ParamEdit.h"

// =================== 全局参数 ===================
int32_t  g_motorSpeed = 0;      // 电机速度
float    g_pidKp      = 1.0f;   // PID比例系数
float    g_pidKi      = 0.0f;   // PID积分系数
float    g_pidKd      = 0.0f;   // PID微分系数
int8_t   g_mode       = 0;      // 运行模式
int8_t   g_mode2      = 0;      // 运行模式2

// =================== 主函数 ===================
void Mymain(void)
{
    Initial_ALL();
    __enable_irq();
		
		Param_AT24C02_Init() ;
    Encoder_Init();

#if PARAM_USE_AT24C02
    // AT24C02初始化：将已注册AT参数从EEPROM恢复到变量中
    Param_AT24C02_Init();
#endif

    // 注册参数表（含步长和类型，AT参数自动从EEPROM加载值）
    Param_Init();
    Param_Register("Speed", &g_motorSpeed, 10.0f, PARAM_INT32);
    Param_Register("Kp",    &g_pidKp,      0.1f,  PARAM_FLOAT);
    Param_Register("Ki",    &g_pidKi,      0.01f, PARAM_FLOAT);
    Param_Register("Kd",    &g_pidKd,      0.01f, PARAM_FLOAT);
    Param_Register("Mode",  &g_mode,       1.0f,  PARAM_INT8);
    Param_Register("Mode2", &g_mode2,      1.0f,  PARAM_INT8);

    while (1)
    {
        // 参数编辑器状态机（长按 KEY1 进入/退出）
        Param_Loop();

        // 非编辑模式下的正常业务逻辑
        if (!Param_IsActive())
        {
            OLED_Printf(0,0,OLED_6X8,"Normal") ;
        }
				
				OLED_Update() ;
    }
}

// Systick定时中断,1ms触发
void HAL_SYSTICK_Callback(void)
{
    Key_Tick();
    LED_Flash_Mode_Tick();
}
