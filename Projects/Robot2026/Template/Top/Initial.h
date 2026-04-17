#ifndef __INITIAL_H
#define __INITIAL_H

#include "Mymain.h"

// Mymain初始化集合,使主函数更简洁
void Initial_ALL(void) ;

// 定时器初始化, 必须放在最后初始化,防止开局访问空指针
void Initial_Timer(void) ;

#endif
