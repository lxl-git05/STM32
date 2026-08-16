# -*- coding: utf-8 -*-
# 字节级安全插入: 在 Mode_5.c 的 KEY_2 单击分支 "// 复位" 注释行之后插入 HAL_NVIC_SystemReset() ;
# 保留 GBK 编码、CRLF 换行和原有空行, 不触碰任何中文注释字节

p = r'D:\github\2-2-STM32\STM32\Projects\Robot2026\Sheng\Car\Mode\Mode_5.c'
data = open(p, 'rb').read()

anchor = b'if (Key_Check(KEY_2,KEY_SINGLE))'
assert data.count(anchor) == 1, 'anchor not unique'
i = data.index(anchor)
brace = data.index(b'{', i)
comment = data.index(b'//', brace)
line_end = data.index(b'\r\n', comment)
close = data.index(b'}', line_end)

# 校验: 注释行之后到块结束之间只有空白 (确认该分支当前为空)
body = data[line_end+2:close]
assert body.strip(b' \t\r\n') == b'', body

new_line = b'\t\tHAL_NVIC_SystemReset() ;\r\n'
new = data[:line_end+2] + new_line + data[line_end+2:]
open(p, 'wb').write(new)

# 打印修改后的分支(GBK 解码)供人工核对
block = new[i-1:close+2]
print(block.decode('gbk', errors='replace'))
print('OK')
