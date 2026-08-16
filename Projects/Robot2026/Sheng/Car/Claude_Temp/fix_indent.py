# -*- coding: utf-8 -*-
# 规整 Mode_5.c KEY_2 单击分支的缩进:
# 用户刚粘贴的 5-tab 缩进 -> 文件风格 2-tab, 并恢复原有空行
# 保留 GBK 编码、CRLF 换行

p = r'D:\github\2-2-STM32\STM32\Projects\Robot2026\Sheng\Car\Mode\Mode_5.c'
data = open(p, 'rb').read()

old = (b'\tif (Key_Check(KEY_2,KEY_SINGLE))\r\n'
       b'\t{\r\n'
       b'\t\t\t\t\t// \xb8\xb4\xce\xbb\r\n'      # // 复位
       b'\t\t\t\t\tHAL_NVIC_SystemReset() ;\r\n'
       b'\t}\r\n')

new = (b'\tif (Key_Check(KEY_2,KEY_SINGLE))\r\n'
       b'\t{\r\n'
       b'\t\t// \xb8\xb4\xce\xbb\r\n'
       b'\t\tHAL_NVIC_SystemReset() ;\r\n'
       b'\t\t\r\n'
       b'\t}\r\n')

assert data.count(old) == 1, 'target block not found or not unique'
data = data.replace(old, new)
open(p, 'wb').write(data)

i = data.index(b'if (Key_Check(KEY_2,KEY_SINGLE))')
j = data.index(b'}', i)
print(data[i-20:j+3].decode('gbk', errors='replace'))
print('OK')
