#include    "Time.h"
#define RTC_RAM_SIZE    56

extern  Time *RTC_getTime();
extern  Time *RTC_displayTime(int x, int y);
extern  Time *Read_Time();
extern  void RTC_Read_RAM(char *values, int size);
extern  void RTC_Write_RAM(char *values, int size);
