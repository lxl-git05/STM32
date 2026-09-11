#ifndef __MODE_G_H
#define __MODE_G_H

#include "main.h"

typedef enum
{
    Mode_Null = 0U,
    Mode_1,
    Mode_2,
    Mode_3,
    Mode_4,
    Mode_5,
    Mode_6,
    Mode_7,
    Mode_8,
    Mode_9,
    Mode_10,
    Mode_End
} Mode_Typedef;

extern Mode_Typedef curr_mode;
extern Mode_Typedef next_mode;

void Mode_To_Next(void);
void Mode_ChangeTo(Mode_Typedef nextmode);

void Mode_G_Setup(void);
void Mode_G_Loop(void);

void Timer_1ms_Callback(void);
void Timer_10ms_Callback(void);
void Timer_20ms_Callback(void);

#endif
