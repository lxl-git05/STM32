#include "bsp_at24c02.h"

/******************************************************************
 * 函 数 名 称：delay_us
 * 函 数 说 明：微秒延时
 * 函 数 形 参：us 延时微秒数
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：STM32F103 @72MHz 约72周期/微秒
******************************************************************/
void delay_us(uint16_t us)
{
    volatile uint32_t count = us * 72 / 5;  // 72MHz / 5 约等于1us（含循环开销）
    while (count--) {
        __NOP();
    }
}

/******************************************************************
 * 函 数 名 称：IIC_Start
 * 函 数 说 明：IIC起始时序
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void IIC_Start(void)
{
    AT24C02_SDA_OUT();

    AT24C02_SDA(1);
    delay_us(10);
    AT24C02_SCL(1);
    delay_us(10);

    AT24C02_SDA(0);
    delay_us(10);
    AT24C02_SCL(0);
    delay_us(10);
}

/******************************************************************
 * 函 数 名 称：IIC_Stop
 * 函 数 说 明：IIC停止信号
 * 函 数 形 参：无
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void IIC_Stop(void)
{
    AT24C02_SDA_OUT();
    AT24C02_SCL(0);
    AT24C02_SDA(0);

    AT24C02_SCL(1);
    delay_us(10);
    AT24C02_SDA(1);
    delay_us(10);
}

/******************************************************************
 * 函 数 名 称：IIC_Send_Ack
 * 函 数 说 明：主机发送应答或者非应答信号
 * 函 数 形 参：0发送应答  1发送非应答
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void IIC_Send_Ack(unsigned char ack)
{
    AT24C02_SDA_OUT();
    AT24C02_SCL(0);
    AT24C02_SDA(0);
    delay_us(10);
    if (!ack) AT24C02_SDA(0);
    else      AT24C02_SDA(1);
    AT24C02_SCL(1);
    delay_us(10);
    AT24C02_SCL(0);
    AT24C02_SDA(1);
}

/******************************************************************
 * 函 数 名 称：I2C_WaitAck
 * 函 数 说 明：等待从机应答
 * 函 数 形 参：无
 * 函 数 返 回：0有应答  1超时无应答
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
unsigned char I2C_WaitAck(void)
{
    unsigned char ack_flag = 10;

    AT24C02_SCL(0);
    AT24C02_SDA(1);        // 释放SDA线
    AT24C02_SDA_IN();      // 切换到输入模式
    delay_us(10);
    AT24C02_SCL(1);
    delay_us(10);

    while ((AT24C02_SDA_GET() == 1) && (ack_flag)) {
        ack_flag--;
        delay_us(10);
    }

    if (ack_flag <= 0) {
        IIC_Stop();
        AT24C02_SDA_OUT();  // 切换回输出模式
        return 1;
    } else {
        AT24C02_SCL(0);
        AT24C02_SDA_OUT();  // 切换回输出模式
    }

    return 0;
}

/******************************************************************
 * 函 数 名 称：Send_Byte
 * 函 数 说 明：写入一个字节
 * 函 数 形 参：dat 要写入的数据
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void Send_Byte(uint8_t dat)
{
    int i = 0;
    AT24C02_SDA_OUT();
    AT24C02_SCL(0);  // 拉低时钟开始数据传输

    for (i = 0; i < 8; i++) {
        AT24C02_SDA((dat & 0x80) >> 7);
        delay_us(1);
        AT24C02_SCL(1);
        delay_us(10);
        AT24C02_SCL(0);
        delay_us(10);
        dat <<= 1;
    }
}

/******************************************************************
 * 函 数 名 称：Read_Byte
 * 函 数 说 明：IIC读时序
 * 函 数 形 参：无
 * 函 数 返 回：读到的数据
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
unsigned char Read_Byte(void)
{
    unsigned char i, receive = 0;

    AT24C02_SDA_IN();  // 切换到输入模式

    for (i = 0; i < 8; i++) {
        AT24C02_SCL(0);
        delay_us(10);
        AT24C02_SCL(1);
        delay_us(10);
        receive <<= 1;
        if (AT24C02_SDA_GET()) {
            receive |= 1;
        }
        delay_us(10);
    }
    AT24C02_SCL(0);

    AT24C02_SDA_OUT();  // 切换回输出模式

    return receive;
}

/******************************************************************
 * 函 数 名 称：AT24C02_WriteByte
 * 函 数 说 明：AT24C02写入一个字节
 * 函 数 形 参：WordAddress 要写入字节的地址  Data 要写入的数据
 * 函 数 返 回：无
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
void AT24C02_WriteByte(unsigned char WordAddress, unsigned char Data)
{
    IIC_Start();
    Send_Byte(AT24C02_ADDRESS_WRITE);
    I2C_WaitAck();
    Send_Byte(WordAddress);
    I2C_WaitAck();
    Send_Byte(Data);
    I2C_WaitAck();
    IIC_Stop();
}

/******************************************************************
 * 函 数 名 称：AT24C02_ReadByte
 * 函 数 说 明：AT24C02读取一个字节
 * 函 数 形 参：WordAddress 要读出字节的地址
 * 函 数 返 回：读出的数据
 * 作       者：LCKFB
 * 备       注：无
******************************************************************/
unsigned char AT24C02_ReadByte(unsigned char WordAddress)
{
    unsigned char Data;

    IIC_Start();
    Send_Byte(AT24C02_ADDRESS_WRITE);
    I2C_WaitAck();
    Send_Byte(WordAddress);
    I2C_WaitAck();
    IIC_Start();
    Send_Byte(AT24C02_ADDRESS_READ);
    I2C_WaitAck();
    Data = Read_Byte();
    IIC_Send_Ack(1);
    IIC_Stop();

    return Data;
}
