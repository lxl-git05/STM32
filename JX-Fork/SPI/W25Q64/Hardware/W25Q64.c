#include "W25Q64.h"
#include "W25Q64_Reg.h"

// ============== W25Q64初始化 ==============
void W25Q64_Init(void)
{
	MySPI_Init() ;
}

// 读取ID号:MID:厂商ID , DID:设备ID
void W25Q64_ReadID(uint8_t *MID , uint16_t *DID)
{
	MySPI_Start() ;
	
	MySPI_SwapByte(W25Q64_JEDEC_ID) ;						// 指令:得到ID号 , 得到的来自从机的信息没用
	
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE) ;	// 得到来自从机的地址,发送的信息没用
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE) ;	
	*DID <<= 8 ;	// 左移,成为高八位
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE) ;	
	
	MySPI_Stop() ;
}

// 写使能
void W25Q64_WriteEnable(void)
{
	MySPI_Start() ;
	MySPI_SwapByte(W25Q64_WRITE_ENABLE) ;
	MySPI_Stop() ;
}

// 等待Busy为0
void W25Q64_WaitBusy(void)
{
	uint32_t timeout = 100000;	// 超时退出
	
	MySPI_Start() ;
	
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1) ;
	
	// 等待循环结束
	while	( (MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01 ) == 0x01) 
	{
		timeout -- ;
		if (timeout == 0)
		{
			break ;
		}
	}
	MySPI_Stop() ;
}

// 页编程
void W25Q64_PageProgram(uint32_t Address , uint8_t *DataArr , uint8_t Count)
{
	// 事实上一次只能写入256个字节,再写就溢出当前扇区了,所以Count使用uint8_t,0-255,刚好256个数据
	
	// 写使能开启!
	W25Q64_WriteEnable() ;
	
	// 开启传输
	MySPI_Start() ;
	
	// 传输指令
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM) ;
	
	// 传输地址
	MySPI_SwapByte(Address >> 16) ;	// 去掉移动的低位,舍弃高位(因为只能传输两位,所以自动舍弃高位)
	MySPI_SwapByte(Address >> 8) ;
	MySPI_SwapByte(Address) ;
	
	// 写入数据,切记这里是<=,就是因为刚好到255
	for(int i = 0 ; i <= Count ; i ++)
	{
		MySPI_SwapByte(DataArr[i]) ;
	}
	
	// 停止
	MySPI_Stop() ;
	
	// 事后等待
	W25Q64_WaitBusy() ;
}

// 页擦除(扇区擦除) : 扇区的地址都是0x_xxx000 -> 0x_xxxFFF
void W25Q64_SectorErase(uint32_t Address)	// 24位地址即可
{
	// 写使能开启!
	W25Q64_WriteEnable() ;
	
	// 开启传输
	MySPI_Start() ;
	
	// 传输指令
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB) ;
	
	// 传输地址
	MySPI_SwapByte(Address >> 16) ;	// 去掉移动的低位,舍弃高位(因为只能传输两位,所以自动舍弃高位)
	MySPI_SwapByte(Address >> 8) ;
	MySPI_SwapByte(Address) ;
	
	// 停止
	MySPI_Stop() ;
	
	// 事后等待
	W25Q64_WaitBusy() ;
}

// 页读取:不像页编程(受RAM缓冲区制约),页读取是没有页数限制的
void W25Q64_ReadData(uint32_t Address , uint8_t *DataArr , uint32_t Count)
{
	// 开启传输
	MySPI_Start() ;
	
	// 传输指令
	MySPI_SwapByte(W25Q64_READ_DATA) ;
	
	// 传输地址
	MySPI_SwapByte(Address >> 16) ;	// 去掉移动的低位,舍弃高位(因为只能传输两位,所以自动舍弃高位)
	MySPI_SwapByte(Address >> 8) ;
	MySPI_SwapByte(Address) ;
	
	// 读取数据
	for(int i = 0 ; i < Count ; i ++ )
	{
		DataArr[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE) ;
	}
	
	// 停止传输
	MySPI_Stop() ;
}

