#include    "RTC.h"

char rtcSec, rtcMin, rtcHour, rtcWeekDay, rtcDay, rtcMonth, rtcYear;
Time rtcTime = {0, 0};

Time *RTC_getTime()
{
    rtcTime.hh = rtcHour;
    rtcTime.mn = rtcMin;
    return &rtcTime;
}

Time *RTC_readTime()
{
    I2C1_Start();
    I2C1_Wr(0xD0);
    I2C1_Wr(0);
    I2C1_Repeated_Start();
    I2C1_Wr(0xD1);

    rtcSec =I2C1_Rd(1);
    rtcTime.mn = rtcMin =I2C1_Rd(1);
    rtcTime.hh = rtcHour =I2C1_Rd(1);
    rtcWeekDay =I2C1_Rd(1);
    rtcDay =I2C1_Rd(1);
    rtcMonth =I2C1_Rd(1);
    rtcYear =I2C1_Rd(0);
    I2C1_Stop();
    return &rtcTime;
}

void RTC_Read_RAM(char *values, int size)
{
    int i;
    I2C1_Start();
    I2C1_Wr(0xD0);
    I2C1_Wr(8);
    I2C1_Repeated_Start();
    I2C1_Wr(0xD1);
    for(i = 0; i < size-1; i++)
        *values++ = I2C1_Rd(1);
    *values = I2C1_Rd(0);
    I2C1_Stop();
}

void RTC_Write_RAM(char *values, int size)
{
    int i;
    I2C1_Start();
    I2C1_Wr(0xD0);
    I2C1_Wr(8);
    for(i = 0; i < size; i++)
        I2C1_Wr(*values++);
    I2C1_Stop();
}


//-------------------- Formats date and time
void Transform_Time(r)
{
  rtcSec  =  ((rtcSec & 0x70) >> 4)*10 + (rtcSec & 0x0F);
  rtcTime.mn = rtcMin  =  ((rtcMin & 0xF0) >> 4)*10 + (rtcMin & 0x0F);
  rtcTime.hh = rtcHour   =  ((rtcHour & 0x30) >> 4)*10 + (rtcHour & 0x0F);
  rtcWeekDay =(rtcWeekDay & 0x07);
  rtcDay  =  ((rtcDay & 0xF0) >> 4)*10 + (rtcDay & 0x0F);
  rtcMonth = ((rtcMonth & 0x10) >> 4)*10 + (rtcMonth & 0x0F);
  rtcYear =  ((rtcYear & 0xF0)>>4)*10+(rtcYear & 0x0F);
}

//-------------------- Output values to LCD
void Display_Time(int x, int y)
{
    /*
    char *dayStr = "";
    if(rtcWeekDay >= 1 && rtcWeekDay <= 7)
        dayStr = days[rtcWeekDay-1];

    Lcd_Chr(x, y++, (rtcDay / 10)   + 48);
    Lcd_Chr(x, y++, (rtcDay % 10)   + 48);
    Lcd_Chr(x, y++, '/');
    Lcd_Chr(x, y++, (rtcMonth / 10)   + 48);
    Lcd_Chr(x, y++, (rtcMonth % 10)  + 48);
    y++;
    */
    Lcd_Chr(x, y++, (rtcHour / 10)   + 48);
    Lcd_Chr(x, y++, (rtcHour % 10)   + 48);
    Lcd_Chr(x, y++, ':');
    Lcd_Chr(x, y++, (rtcMin / 10) + 48);
    Lcd_Chr(x, y++, (rtcMin % 10) + 48);
    Lcd_Chr(x, y++, ':');
    Lcd_Chr(x, y++, (rtcSec / 10) + 48);
    Lcd_Chr(x, y++, (rtcSec % 10) + 48);

}

Time *RTC_displayTime(int x, int y)
{
    Time *t;
    t = RTC_readTime();
    Transform_Time();
    Display_Time(x, y);
    return t;
}
