#ifndef __HMC5883_REG_H
#define __HMC5883_REG_H

// ******************** HMC5883L 寄存器地址宏定义 ********************
#define HMC5883L_ADDR           0x1E    // I2C 7位地址（实际写时左移1位：0x3C）

#define HMC5883L_CONFIG_A       0x00    // 配置寄存器 A
#define HMC5883L_CONFIG_B       0x01    // 配置寄存器 B（增益设置）
#define HMC5883L_MODE           0x02    // 模式寄存器
#define HMC5883L_DATA_X_MSB     0x03    // X轴数据高字节（数据输出从这里开始）
#define HMC5883L_DATA_X_LSB     0x04
#define HMC5883L_DATA_Z_MSB     0x05
#define HMC5883L_DATA_Z_LSB     0x06
#define HMC5883L_DATA_Y_MSB     0x07
#define HMC5883L_DATA_Y_LSB     0x08
#define HMC5883L_STATUS         0x09    // 状态寄存器
#define HMC5883L_ID_A           0x0A    // 识别寄存器 A（应读出 'H' = 0x48）
#define HMC5883L_ID_B           0x0B    // 识别寄存器 B（应读出 '4' = 0x34）
#define HMC5883L_ID_C           0x0C    // 识别寄存器 C（应读出 '3' = 0x33）

// ******************** 配置寄存器 A (0x00) 相关宏 ********************
// 采样平均次数
#define HMC5883L_SAMPLES_1      0x00    // 1次平均
#define HMC5883L_SAMPLES_2      0x20    // 2次平均
#define HMC5883L_SAMPLES_4      0x40    // 4次平均
#define HMC5883L_SAMPLES_8      0x60    // 8次平均（推荐，默认）

// 数据输出速率
#define HMC5883L_ODR_0_75HZ     0x00    // 0.75 Hz
#define HMC5883L_ODR_1_5HZ      0x04
#define HMC5883L_ODR_3HZ        0x08
#define HMC5883L_ODR_7_5HZ      0x0C
#define HMC5883L_ODR_15HZ       0x10    // 常用
#define HMC5883L_ODR_30HZ       0x14
#define HMC5883L_ODR_75HZ       0x18
#define HMC5883L_ODR_160HZ      0x1C    // 最大速率

// 测量模式（正常/正偏/负偏）
#define HMC5883L_MEAS_NORMAL    0x00    // 正常测量
#define HMC5883L_MEAS_POS_BIAS  0x01    // 正偏置（用于自检）
#define HMC5883L_MEAS_NEG_BIAS  0x02    // 负偏置

// ******************** 配置寄存器 B (0x01) 增益设置 ********************
#define HMC5883L_GAIN_1370      0x00    // ±0.88 Ga  (推荐室内使用)
#define HMC5883L_GAIN_1090      0x20    // ±1.3  Ga  (默认，最常用)
#define HMC5883L_GAIN_820       0x40    // ±1.9  Ga
#define HMC5883L_GAIN_660       0x60    // ±2.5  Ga
#define HMC5883L_GAIN_440       0x80    // ±4.0  Ga
#define HMC5883L_GAIN_390       0xA0    // ±4.7  Ga
#define HMC5883L_GAIN_330       0xC0    // ±5.6  Ga
#define HMC5883L_GAIN_230       0xE0    // ±8.1  Ga  (最大量程)

// ******************** 模式寄存器 (0x02) ********************
#define HMC5883L_MODE_CONTINUOUS 0x00   // 连续测量模式（常用）
#define HMC5883L_MODE_SINGLE     0x01   // 单次测量模式
#define HMC5883L_MODE_IDLE       0x02   // 空闲模式
//#define HMC5883L_MODE_SLEEP      0x03   // 睡眠模式 , 没有的

// ********** 重要: 确定想要的配置（可根据需求修改） **********
#define HMC5883L_SAMPLES        HMC5883L_SAMPLES_8      // 8次平均，抗噪好
#define HMC5883L_OUTPUT_RATE    HMC5883L_ODR_15HZ       // 15Hz 常用
#define HMC5883L_GAIN           HMC5883L_GAIN_1090      // ±1.3 Ga（默认，适合大多数场景）
#define HMC5883L_OPERATING_MODE HMC5883L_MODE_CONTINUOUS

// ******************** 灵敏度宏（用于原始数据转物理单位） ********************
#if HMC5883L_GAIN == HMC5883L_GAIN_1370
    #define MAG_SENSITIVITY_XY 1370.0f
    #define MAG_SENSITIVITY_Z  1370.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_1090
    #define MAG_SENSITIVITY_XY 1090.0f
    #define MAG_SENSITIVITY_Z   980.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_820
    #define MAG_SENSITIVITY_XY  820.0f
    #define MAG_SENSITIVITY_Z   760.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_660
    #define MAG_SENSITIVITY_XY  660.0f
    #define MAG_SENSITIVITY_Z   600.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_440
    #define MAG_SENSITIVITY_XY  440.0f
    #define MAG_SENSITIVITY_Z   400.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_390
    #define MAG_SENSITIVITY_XY  390.0f
    #define MAG_SENSITIVITY_Z   355.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_330
    #define MAG_SENSITIVITY_XY  330.0f
    #define MAG_SENSITIVITY_Z   295.0f
#elif HMC5883L_GAIN == HMC5883L_GAIN_230
    #define MAG_SENSITIVITY_XY  230.0f
    #define MAG_SENSITIVITY_Z   205.0f
#else
    #error "Invalid gain setting!"
#endif


// 注意：HMC5883L 的 X、Y、Z 轴灵敏度相同（不像某些芯片轴向不同）

#endif
