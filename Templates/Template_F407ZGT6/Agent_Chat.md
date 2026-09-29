现在你需要帮我构建模板工程

你首先参考D:\github\HUST_STI\HUST_STI_1\Templete\Templete_STM32F407ZGT6

本工程

+ 你不需要实现 Mysystem
+ 硬件ISR可以把接口或者__Week放入MyISR，我要统一管理
+ 帮我根据README-F4.md配置和D:\github\HUST_STI\HUST_STI_1\Templete\Templete_STM32F407ZGT6一样的逻辑
+ 比如OLED和Key，不再需要Mysystem，直接改成HAL操作
+ Mymain和Mode等架构还是保持逻辑不变
+ 不要再在本工程新增其他新的硬件了，就基于当前资源，实现和示例工程相同的逻辑即可

+ AT等保存操作也别搞，反正我在md没提到的硬件都不要搞
+ 最后实现完成之后在Mode2实现：
  + 要求：最简实现，别搞这么麻烦
  + 单击按键1，num *= 10
  + 双击按键1，num / = 10
  + 单击按键2，清零
  + OLED显示num
  + Serial在中断实现打印num : Serial_printf("%d\n",num) ;