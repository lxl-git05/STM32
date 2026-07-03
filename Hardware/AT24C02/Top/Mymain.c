#include "Mymain.h"
#include "Initial.h"
// =================== 全局变量 ===================
#include "bsp_at24c02.h"
#include "at24c02_manager.h"

// =================== AT24C02 参数定义 ===================
// 需要用EEPROM保存的参数
int16_t kp = 100;
int16_t ki = 10;
int16_t kd = 0;
float target_speed = 50.0f;
uint8_t mode = 1;

// 参数注册
AT24C02_REG(kp,           AT24C02_I16);
AT24C02_REG(ki,           AT24C02_I16);
AT24C02_REG(kd,           AT24C02_I16);
AT24C02_REG(target_speed, AT24C02_F);
AT24C02_REG(mode,         AT24C02_U8);

// 注册表
AT24C02_Item_t* at24c02_table[] = {
    &kp_item,
    &ki_item,
    &kd_item,
    &target_speed_item,
    &mode_item,
};
uint8_t at24c02_table_size = 5;

// =================== I2C 扫描函数 ===================
/**
 * @brief 扫描所有I2C从机地址
 * @param start_addr 起始地址
 * @param end_addr 结束地址
 * @return 找到的从机数量
 */
uint8_t I2C_Scan(uint8_t start_addr, uint8_t end_addr)
{
    uint8_t count = 0;
    uint8_t found_addr[128];
    uint8_t addr;

    OLED_Clear();
    OLED_Printf(0, 0, OLED_6X8, "I2C Scan...");
    OLED_Printf(0, 10, OLED_6X8, "SCL:PB14 SDA:PB15");
    OLED_Update();
    HAL_Delay(2000);  // 等待2秒让用户准备

    for (addr = start_addr; addr <= end_addr; addr++) {
        // 尝试发送起始信号和地址
        IIC_Start();
        AT24C02_SDA_OUT();
        Send_Byte(addr);  // 发送地址+写位

        if (I2C_WaitAck() == 0) {
            // 收到ACK，设备存在
            found_addr[count++] = addr;
            OLED_Printf(0, 30, OLED_6X8, "Found:0x%02X!", addr);
            OLED_Update();
        }

        IIC_Stop();
        HAL_Delay(10);  // 间隔10ms再扫描下一个
    }

    // 显示结果
    OLED_Clear();
    if (count == 0) {
        OLED_Printf(0, 20, OLED_6X8, "No device found!");
        OLED_Printf(0, 30, OLED_6X8, "Check wiring!");
    } else {
        OLED_Printf(0, 0, OLED_6X8, "Found %d devices:", count);
        for (uint8_t i = 0; i < count && i < 4; i++) {
            OLED_Printf(0, 10 + i * 10, OLED_6X8, "0x%02X (W:0x%02X)", found_addr[i], found_addr[i]);
        }
    }
    OLED_Update();

    return count;
}

// =================== 实例代码 ===================

void Mymain(void)
{
    Initial_ALL();
    __enable_irq();

    // I2C 扫描测试
    I2C_Scan(0x00, 0x7F);

    // 等待
    while(1) {
        HAL_Delay(1000);
    }

/*
    // AT24C02 初始化
    AT24C02_Manager_Init();
    AT24C02_LoadAll();

    while(1)
    {
        // 按键1单击：从EEPROM重新加载所有参数
        if (Key_Check(KEY_1, KEY_SINGLE))
        {
            AT24C02_LoadAll();
        }

        // 按键2单击：保存mode和kd到EEPROM
        if (Key_Check(KEY_2, KEY_SINGLE))
        {
            AT24C02_SaveOne(&mode);
            AT24C02_SaveOne(&kd);
        }

        // 按键1+2同时按：格式化EEPROM并保存所有参数
        if (Key_Check(KEY_1, KEY_HOLD) && Key_Check(KEY_2, KEY_HOLD))
        {
            AT24C02_Format();
            AT24C02_SaveAll();
            AT24C02_LoadAll();
        }

        // OLED显示AT24C02参数（并排显示）
        OLED_Printf(0, 0,  OLED_6X8, "KP:%d", kp);
        OLED_Printf(0, 10, OLED_6X8, "KI:%d", ki);
        OLED_Printf(0, 20, OLED_6X8, "KD:%d", kd);
        OLED_Printf(0, 30, OLED_6X8, "MODE:%d", mode);

        OLED_Update();
    }
*/
}

// Systick定时中断,1ms触发
void HAL_SYSTICK_Callback(void)
{
    Key_Tick();
    LED_Flash_Mode_Tick();
}
