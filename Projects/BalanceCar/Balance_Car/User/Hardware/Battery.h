#ifndef USER_BATTERY_H
#define USER_BATTERY_H

void Battery_Init(void);
/* Returns 0..100 percent, or -1 if ADC conversion fails. */
int Battery_GetValue(void);

#endif /* USER_BATTERY_H */
