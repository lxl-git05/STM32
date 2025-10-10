[toc]

## 1. 按格式从字符串中读取数据 - `sscanf()`

```c
int sscanf(const char *str, const char *format, ...);
```

注意:

从字符串中==**按格式**==读取数据

示例:

### 1-1 从字符串中读取数据

```c
#include <stdio.h>

int main(void)
{
    char data[] = "12 3.45";
    int a;
    float b;

    sscanf(data, "%d %f", &a, &b);	// !!! 这里就是按格式读取了数据(加了一个空格) !!!

    printf("a=%d, b=%.2f\n", a, b);
    return 0;
}
```

### 1-2 从带文本的字符串中读取数据

```c
char str[] = "X=10,Y=25";
int x, y;
sscanf(str, "X=%d,Y=%d", &x, &y);	// !!! 这里也是按格式读取数据 !!!
printf("x=%d y=%d\n", x, y);
```

### 1-3 读取字符串

```c
char str[] = "CMD:LED_ON";
char cmd[20];
sscanf(str, "CMD:%s", cmd);			// !!! 这里也是按格式读取数据 !!!
printf("cmd = %s\n", cmd);
```



## 2. 从字符串中查询字符or者子串

| 函数             | 功能                                       | 示例                                          |
| :--------------- | ------------------------------------------ | --------------------------------------------- |
| `strchr(s, c)`   | 查找字符 `c` 在字符串 `s` 中首次出现的位置 | `strchr("abc:def", ':')` 返回指向 `:` 的指针  |
| `strstr(s1, s2)` | 查找子串 `s2` 在 `s1` 中的位置             | `strstr("CMD:LED_ON", "LED")` 返回 `"LED_ON"` |



## 3. 不按格式解析整数or浮点数

| 函数       | 功能                                 |
| ---------- | ------------------------------------ |
| `strtol()` | 把字符串转换为长整数（支持不同进制） |
| `strtod()` | 把字符串转换为浮点数                 |

例1:

```c
char str[] = "0xFF";
long val = strtol(str, NULL, 0); // 自动判断进制
printf("val = %ld\n", val);

```

例2:

```c
char str[] = "123.45";
double val = strtod(str, NULL);
printf("val = %.2f\n", val);

```

