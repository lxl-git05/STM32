# SPI原理与应用

[toc]

## 1. 通信线路

![image-20260115224324138](SPI通信.assets/image-20260115224324138.png)

* MISO : Master Input Slave Output
* MOSI : Master Output Slave Input 

* SS : 位选,选择通信对象(一主多从)

* SCK : 时钟线

## 2. 数据传输原理 : 移位

![image-20260115224454049](SPI通信.assets/image-20260115224454049.png)

## 3. ==时序基本单元==

### 3-1 起始与终止

* 起始条件: SS从H变成L
* 终止条件: SS从L变成H

### 3-2 字节交换

==CPOL : (Clock Polarity)== : 时钟极性 0 / 1

==CPHA :（Clock Phase）== : 时钟相位 0 / 1

* **所以有四种模式**,==重点了解模式0即可==





***

* 交换一个字节（==模式1==）

* CPOL=0：空闲状态时，SCK为低电平

* CPHA=1：==SCK第一个边沿(如上升沿)移出数据，第二个边沿(如下降沿)移入数据==

![image-20260115225944662](SPI通信.assets/image-20260115225944662.png)

***

* 交换一个字节（==模式0==）

* CPOL=0：空闲状态时，SCK为低电平

* CPHA=0：SCK第一个边沿移入数据，第二个边沿移出数据



* 和模式1的区别:
  * 模式0在第一个边沿就需要移入数据,但是此时还没有数据移出,**所以在SS下降沿就进行一位移出**

![image-20260115230710771](SPI通信.assets/image-20260115230710771.png)

***

* 交换一个字节（模式2）

* CPOL=1：空闲状态时，SCK为高电平

* CPHA=0：SCK第一个边沿移入数据，第二个边沿移出数据

***

* 交换一个字节（模式3）

* CPOL=1：空闲状态时，SCK为高电平

* CPHA=1：SCK第一个边沿移出数据，第二个边沿移入数据



### 3-3 具体电路沿(模式0)

#### 3-3-1 发送单个指令

•发送指令

•向SS指定的设备，发送指令（0x06）

![image-20260115231042082](SPI通信.assets/image-20260115231042082.png)

* SCK上升沿对应写入数据(在SS下降沿的时候就开始输出(写出)数据了)

* SCK下降沿就开始写出数据

* 效果:主机写入0x06,从机交换到0xFF(垃圾数据,从机其实也不会鸟他)

#### 3-3-2 写入数据(指令 + 地址 + 数据)

•指定地址写

•向SS指定的设备，发送写指令（0x02），

 随后在指定地址（Address[23:0]）下，写入指定数据（Data）

![image-20260115231431989](SPI通信.assets/image-20260115231431989.png)

* 时序效果 : 0x02发送写指令 + 地址(高低位一共24位)0x12(23:16) + 0x34(15:8) + 0x56(7:0) + 

8位数据:0x55 , 即 指令(8) + 地址(24) + 数据(**自己指定长度**(==SPI含有地址自增寻址,可以写入多位数据==))



#### 3-3-3 指定地址读(指令 + 地址 + 交换数据)

•指定地址读

•向SS指定的设备，发送读指令（0x03），

随后在指定地址（Address[23:0]）下，读取从机数据（Data）

![image-20260115232003785](SPI通信.assets/image-20260115232003785.png)

* 时序效果:
  * 指令(0x03) : 读取数据指令
  * 地址(23:0) : 0x123456 : 地址
  * 数据(此时就是主机给垃圾值:0xFF等) , 从机交换来好东西:数据0x55
  * ==同样也有地址寻址自增,实现连续读==



## 4. **FLash操作注意事项**

写入操作时：

•==写入操作前，必须先进行写使能==

•每个数据位只能由1改写为0，不能由0改写为1(**导致下面的解决方案**)

•==写入数据前必须先擦除，擦除后，所有数据位变为1==

•擦除必须按最小擦除单元进行(一个扇区:4096个字节)

•连续写入多字节时，最多写入一页的数据，超过页尾位置的数据，会回到页首覆盖写入(因为RAM存不下了)

•==写入操作结束后，芯片进入忙状态，不响应新的读写操作==,所以需要进行忙状态判断,可以是写入数据之后while等待忙状态结束,也可以是读取等操作之前先判断是否是忙状态

读取操作时：

•直接调用读取时序，无需使能，无需额外操作，没有页的限制，读取操作结束后不会进入忙状态，**但不能在忙状态时读取**

## 5. 软件SPI时序模拟代码(模式0)

### 5-1 初始化

```c
// 软件SPI在HAL已经进行了初始化:
//#define SPI_SS_Pin GPIO_PIN_4
//#define SPI_SS_GPIO_Port GPIOA

//#define SPI_CLK_Pin GPIO_PIN_5
//#define SPI_CLK_GPIO_Port GPIOA

//#define SPI_DO_Pin GPIO_PIN_6
//#define SPI_DO_GPIO_Port GPIOA

//#define SPI_DI_Pin GPIO_PIN_7
//#define SPI_DI_GPIO_Port GPIOA
// 记得SCK作为时钟先是0(从而满足模式0的第一个沿为上升沿)
// SS最开始为1,表示不选中(因为是低电平有效)
```

* 四个GPIO任意指定
  * SS -> 位选,推挽输出
  * CLK -> 时钟,推挽输出
  * DI -> MOSI , Master输出数据线,推挽输出
  * DO -> MISO , Master输入数据线,上拉输入

### 5-2 引脚操作

```c
// 引脚操作:SS写入
void MySPI_W_SS(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SPI_SS_GPIO_Port , SPI_SS_Pin , (GPIO_PinState)BitValue) ;
}

// 引脚操作:SCK写入
void MySPI_W_SCK(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SPI_CLK_GPIO_Port , SPI_CLK_Pin , (GPIO_PinState)BitValue) ;
}

// 引脚操作:MOSI(DI)写入
void MySPI_W_MOSI(uint8_t BitValue)
{
	HAL_GPIO_WritePin(SPI_DI_GPIO_Port , SPI_DI_Pin , (GPIO_PinState)BitValue) ;
}

// 引脚操作:MISO(DO)读取
uint8_t MySPI_R_MISO(void)
{
	return HAL_GPIO_ReadPin(SPI_DO_GPIO_Port , SPI_DO_Pin) ;
}
```



### 5-3 时序模拟

* 开启 / 关闭 SPI通信
  * 记住SPI是一主多从,但是每次只能开启一个从机进行通信,否则会导致硬件短路等竞争,所以在一个从机工作时,其他从机必须SS = 1 , 并且MISO也就是==数据输入线为高阻态!==

```c
// 开启SPI通信
void MySPI_Start(void)
{
	MySPI_W_SS(0) ;	// 低有效,所以是0开启
}
// 结束SPI通信
void MySPI_Stop(void)
{
	MySPI_W_SS(1) ;	// 高无效
}
```

* 交换一个字节 , ==核心逻辑==
  * 原理在于每次数据传输进行8位数据的移位处理
  * 该函数运行的前提的SS已经置为0,产生了一个下降沿,从而在模式0的情况下,从机已经在输入线输入了1bit数据

```c
// 交换一个字节,主机发送ByteSend,接收返回值,本函数实现的是模式0功能,改为模式1,2,3自行研究
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0x00 ;
	// 在for循环之前有SS下降沿,所以从机早就准备好了输出数据,并且存入到了移位寄存器,所以for_loop一开始就需要输出数据
	for (int i = 0 ; i < 8 ; i ++)
	{
		MySPI_W_MOSI( ByteSend & (0x80 >> i) ) ;	// 传输最高位给从机
		MySPI_W_SCK(1) ;	// 发出上升沿,此时从机发送从高位,主机下一步去读取
		if (MySPI_R_MISO() == 1) { ByteReceive |= (0x80 >> i) ; }
		MySPI_W_SCK(0) ;	// 下降沿,主机从机输出下一位数据到移位寄存器
	}
	
	return ByteReceive ;
}
```

## 6. ==W25Q64的SPI应用驱动==

* ==要知道SPI对应的W25Q64指令集,每一条指令只且仅只对应一个SS下降 + 上升沿 , 所以执行多条指令就得多次开关SS从机!==
* 其他的外设当然有不同的指令集处理方法,详见手册

***

* 初始化 , 其实也就是SPI时序初始化

```c
void W25Q64_Init(void)
{
	MySPI_Init() ;
}
```

***

* 读取ID号:MID:厂商ID , DID:设备ID

  * 核心在于 : **读** , 所以需要一下流程
  * ==使能从机 -> 发送指令 -> 发送地址 -> 得到数据 -> 失能从机 (这就是读数据的完整过程)==

  * 由于ID号有两个 , 一个是MID,8bits , 一个是DID , 16bits , 所以使用u8和u16接收

```c
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
```

***

* 写使能: 每次写数据之前都必须进行写使能
  * 本质上仅仅是**发指令** 
  * 流程:
  * ==使能从机 -> 发送指令 -> 失能从机====(这是仅仅发送指令的完整过程)==

```c
// 写使能
void W25Q64_WriteEnable(void)
{
	MySPI_Start() ;
	MySPI_SwapByte(W25Q64_WRITE_ENABLE) ;
	MySPI_Stop() ;
}
```

***

* 判断Busy状态,等待Busy为0
  * 本质上也是**读取数据**

```c
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
```

***

* 页编程(1page操作)
  * 编程是写操作,需要按页!!!
  * **一个page等于2048bit = 256Byte = 64 * 32个int32_t = 64 * 32个int**
  * 对于电赛级工程存储64个int基本上OK了,并且W25Q64含有32,768个page, 完全OK
  * 编程是对page进行操作的,擦除可以是page,可以是sector等,但是page一般就行了
  * **记得先   写使能!    **
  * **写操作流程**
  * ==开启写使能(也是一个指令模型,内含开启-指令-关闭全过程) -> <u>开启传输 -> 传输指令 -> 传输地址 -> 写入数据 -> 停止传输 -> 事后等待,直到busy结束</u>==

```c
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
```

***

* 页擦除(16page操作,已经是最小单位了)

  * 0x_xxx000 -> 0x_xxx_FFF , 对应就是一个扇区,==一擦就是16个page==

  * 一般给地址是低位地址,也就是0x_xxx_000,这样可以刚好擦除16个page

```c
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
```

***

* 页读取

```c
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
```

## 7. 硬件SPI

* 参考资料[【STM32】HAL库 STM32CubeMX教程十四---SPI_cubemx spi-CSDN博客](https://blog.csdn.net/as480133937/article/details/105849607?ops_request_misc=elastic_search_misc&request_id=4d2e54a43451345ab59547c46cf5c202&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~top_positive~default-1-105849607-null-null.142^v102^pc_search_result_base3&utm_term=stm32 HAL SPI &spm=1018.2226.3001.4187)

***

* 要点
  * SS位选最好还是软件模拟,毕竟只是起一个选择功能,没别的卵用,软件反而更灵活,具体配置见Cube
  * HAL库有三种方式实现SPI,轮询,中断,DMA,只展示第一种,剩下两种未来需要精益求精再考虑使用

***

* 基本改动

```c
// 交换一个字节,主机发送ByteSend,接收返回值,本函数实现的是模式0功能,改为模式1,2,3自行研究
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0x00 ;
	// ============= 软件SPI ============= 
	#ifdef SPI_Software_Mode_Enable
	// 在for循环之前有SS下降沿,所以从机早就准备好了输出数据,并且存入到了移位寄存器,所以for_loop一开始就需要输出数据
	for (int i = 0 ; i < 8 ; i ++)
	{
		MySPI_W_MOSI( ByteSend & (0x80 >> i) ) ;	// 传输最高位给从机
		MySPI_W_SCK(1) ;	// 发出上升沿,此时从机发送从高位,主机下一步去读取
		if (MySPI_R_MISO() == 1) { ByteReceive |= (0x80 >> i) ; }
		MySPI_W_SCK(0) ;	// 下降沿,主机从机输出下一位数据到移位寄存器
	}
	#else
	// ============= 硬件SPI =============
	HAL_SPI_TransmitReceive( &hspi1 , &ByteSend , &ByteReceive , 1 , 100000 );
	#endif
	
	return ByteReceive;
	
}
```

## 8. SPI软硬件Cube配置

### 1. 软件模拟SPI

![image-20260116141553531](D:/github/2-2-STM32/Tools/0-Cube配置/Cube配置表.assets/image-20260116141553531.png)

* SS初始为HIGH,也就是默认先不选中从机
* CLK初始为LOW,也就是第一个边沿为上升沿,**满足SPI的模式0**
* 其他GPIO的初始状态H,L都行

### 2. 硬件SPI

* 全双工模式
* 关闭硬件NSS(也就是软件模拟SS,和上面的software一样),所以待会需要自己配置软件模拟SS

![image-20260116142233317](D:/github/2-2-STM32/Tools/0-Cube配置/Cube配置表.assets/image-20260116142233317.png)

***

![image-20260116142447464](D:/github/2-2-STM32/Tools/0-Cube配置/Cube配置表.assets/image-20260116142447464.png)

***

* 配置软件SPI_SS

![image-20260116142542353](D:/github/2-2-STM32/Tools/0-Cube配置/Cube配置表.assets/image-20260116142542353.png)

* 初始为H,也就是默认不选中!



