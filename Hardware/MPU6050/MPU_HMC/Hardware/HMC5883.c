#include "HMC5883.h"
#include "LED_Flash.h"

HMC_Data_Typedef HMC_Data;

// 初始化HMC磁力计
void HMC_Init(void)
{
	uint8_t configA = HMC5883L_SAMPLES | HMC5883L_OUTPUT_RATE | HMC5883L_MEAS_NORMAL;
	uint8_t configB = HMC5883L_GAIN;
	uint8_t mode    = HMC5883L_OPERATING_MODE;

	// 写入配置寄存器A：采样次数 + 输出速率 + 测量模式
	HMC5883_WriteReg(HMC5883L_CONFIG_A, configA);

	// 写入配置寄存器B：增益
	HMC5883_WriteReg(HMC5883L_CONFIG_B, configB);

	// 写入模式寄存器：连续测量模式
	HMC5883_WriteReg(HMC5883L_MODE, mode);
}

// 写入数据
void HMC5883_WriteReg(uint8_t RegAddress , uint8_t Data)
{
	// 软件I2C
	#ifdef HMC5883_IS_Soft_Drive
	// 开始信号
	MyI2C_Start() ;	
	// 第1个字节(从机地址)
	MyI2C_SendByte(HMC5883_ADDRESS) ;	// 定位从机地址
	MyI2C_ReceiveAck() ;	// 得到应答位,后续可以处理,这里没用
	
	// 第2个字节(从机寄存器地址)
	MyI2C_SendByte(RegAddress) ;			// 定位从机寄存器地址
	MyI2C_ReceiveAck() ;	// 得到应答位
	
	// 第3个字节(写入的数据),可for_loop从数组中写入多个字节
	MyI2C_SendByte(Data) ;			// 写入的数据
	MyI2C_ReceiveAck() ;	// 得到应答位
	// 停止信号
	MyI2C_Stop() ;
	#else
	// HAL库硬件IIC
	//或者直接指定地址写：
	HAL_I2C_Mem_Write(hi2c_HMC5883, HMC5883_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, 10000);
	#endif
}

// 读取数据
uint8_t HMC5883_ReadReg(uint8_t RegAddress)
{
	// 读取的数据
	uint8_t Data ;
	
//	IIC软件
	#ifdef HMC5883_IS_Soft_Drive
	// 开始信号
	MyI2C_Start() ;	
	// 第1个字节(从机地址)
	MyI2C_SendByte(HMC5883_ADDRESS) ;	// 定位从机地址为写
	MyI2C_ReceiveAck() ;	// 得到应答位,后续可以处理,这里没用
	// 第2个字节(从机寄存器地址)
	MyI2C_SendByte(RegAddress) ;			// 定位从机寄存器地址
	MyI2C_ReceiveAck() ;	// 得到应答位
	
	// 重新开始信号(Sr)
	MyI2C_Start() ;	
	// 再次第1个字节(从机地址)
	MyI2C_SendByte(HMC5883_ADDRESS | 0x01) ;	// 定位从机地址为读
	MyI2C_ReceiveAck() ;	// 得到应答位,后续可以处理,这里没用
	
	// 接收1个字节,可for_loop接收多个字节存在数组里面
	Data = MyI2C_ReceiveByte() ;
	MyI2C_SendAck(1) ; 		// 发送应答为:无应答,使得从机交出SDA控制权
	// 停止信号
	MyI2C_Stop() ;	
	// HAL库硬件IIC
	#else
	HAL_I2C_Mem_Read(hi2c_HMC5883, HMC5883_ADDRESS, RegAddress, I2C_MEMADD_SIZE_8BIT, &Data, 1, 10000);
	#endif
	// 返回数据
	return Data ;
}
// 更新数据,读取HMC5883连续三轴数据
void HMC5883_Update_Data(void)
{
    uint8_t buf[6]; // 连续读取 X_MSB ~ Y_LSB 共 6 字节

    #ifdef HMC5883_IS_Soft_Drive
    // 软件I2C读取
    MyI2C_Start();
    MyI2C_SendByte(HMC5883_ADDRESS); // 写模式
    MyI2C_ReceiveAck();

    MyI2C_SendByte(HMC5883L_DATA_X_MSB); // 起始寄存器
    MyI2C_ReceiveAck();

    MyI2C_Start();
    MyI2C_SendByte(HMC5883_ADDRESS | 0x01); // 读模式
    MyI2C_ReceiveAck();

    for(uint8_t i = 0; i < 6; i++)
    {
        buf[i] = MyI2C_ReceiveByte();
        if(i == 5)
            MyI2C_SendAck(1); // NACK
        else
            MyI2C_SendAck(0); // ACK
    }
    MyI2C_Stop();

    #else
    // HAL库硬件I2C
    uint8_t retry = 5;
    while(retry--)
    {
        if(HAL_I2C_Mem_Read(hi2c_HMC5883, HMC5883_ADDRESS, HMC5883L_DATA_X_MSB,
                            I2C_MEMADD_SIZE_8BIT, buf, 6, 1000) == HAL_OK)
        {
            break;
        }
        else
        {
            ;
        }
    }
    #endif

    // HMC5883 输出顺序 X-Z-Y，拼接成 int16_t
    HMC_Data.MagX_Raw = (int16_t)((buf[0] << 8) | buf[1]);
    HMC_Data.MagZ_Raw = (int16_t)((buf[2] << 8) | buf[3]);
    HMC_Data.MagY_Raw = (int16_t)((buf[4] << 8) | buf[5]);
}

// 数据校准
void HMC5883_Calibrate(void)
{
    int16_t mag_max[3] = {-32768, -32768, -32768};
    int16_t mag_min[3] = {32767, 32767, 32767};
    
    uint32_t start_time = HAL_GetTick();
		
    while(HAL_GetTick() - start_time < 20000)  // 20秒校准时间
    {
        HMC5883_Update_Data();  // 更新数据
        
        // 更新最大最小值（X, Z, Y 对应索引0,1,2）
        if(HMC_Data.MagX_Raw > mag_max[0]) mag_max[0] = HMC_Data.MagX_Raw;
        if(HMC_Data.MagX_Raw < mag_min[0]) mag_min[0] = HMC_Data.MagX_Raw;
        
        if(HMC_Data.MagZ_Raw > mag_max[1]) mag_max[1] = HMC_Data.MagZ_Raw;
        if(HMC_Data.MagZ_Raw < mag_min[1]) mag_min[1] = HMC_Data.MagZ_Raw;
        
        if(HMC_Data.MagY_Raw > mag_max[2]) mag_max[2] = HMC_Data.MagY_Raw;
        if(HMC_Data.MagY_Raw < mag_min[2]) mag_min[2] = HMC_Data.MagY_Raw;
        
        HAL_Delay(50);  // 约20Hz采样
    }
    
		LED_Flash_Mode_Set_Mode(LED_Flash_ON) ;
		
    // 计算偏移（硬铁校准）
    HMC_Data.MagX_Offset = (mag_max[0] + mag_min[0]) / 2;
    HMC_Data.MagZ_Offset = (mag_max[1] + mag_min[1]) / 2;
    HMC_Data.MagY_Offset = (mag_max[2] + mag_min[2]) / 2;
}
