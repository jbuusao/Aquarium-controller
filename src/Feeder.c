#include    "Aquarium6.h"
#include    "Feeder.h"
#include    "LCD.h"
#include    "RTC.h"
#include    "EEPROM_24LC256.h"

#define     COUNTER_ADDR    0
#define     BASE_ADDR   2

char *title = "Rep. ";
char *saved = "SAVED          ";
char *column = ":";
#define TITLE_LENGTH 5


Time feedTime[2] = {{0,0},{0,0}};
Time testTime;
signed char feed1_hh = 99;
signed char feed1_mn = 99;

void loadFeederData()
{
    RTC_Read_RAM((char *)feedTime, 2*sizeof(Time));
}

void saveFeederData()
{
    RTC_Write_RAM((char *)feedTime, 2*sizeof(Time));
    Lcd_Out(2, 1, saved);
}

int Feeder_test(Time *time)
{
    int i;
    if(time->hh == testTime.hh && time->mn == testTime.mn)
        return 0;
    testTime.hh = time->hh;
    testTime.mn = time->mn;
    for(i = 0; i < 2; i++)
    {
        if(time->mn == 59)
        {
            return 1;
        }
        if(!(testTime.hh == 0 && testTime.mn == 0))
        {
          if(feedTime[i].hh == time->hh && feedTime[i].mn == time->mn)
          {
              return 1;
          }
        }
    }
    return 0;
}

char *timeStr(signed char value)
{
    temp_text[0] = (value / 10) + '0';
    temp_text[1] = (value % 10) + '0';
    temp_text[2] = 0;
    return temp_text;
}

void displayFeedTimes(int x, int y)
{
    Time *t;
    int i;
    t = &feedTime;

    clearLine(x);
    Lcd_Out(x, y, title);
    y += TITLE_LENGTH;

    for(i = 0; i < 2; i++, t++)
    {
      Lcd_Out(x, y, timeStr(t->hh));
      Lcd_Out(x, y + 2, column);
      Lcd_Out(x, y + 3, timeStr(t->mn));
      y += 6;
    }
}

void displayEditFeed1(int mode, int index, int x, int y)
{
    Time *t;
    t = &feedTime[index];
    temp_text[0] = index + 1 + '0';
    temp_text[1] = ':';
    temp_text[2] = 0;

    clearLine(x);
    Lcd_Out(x, y, title);
    Lcd_Out(x, y + TITLE_LENGTH, temp_text);
    Lcd_Out(x, y + TITLE_LENGTH + 3, timeStr(t->hh));
    Lcd_Out(x, y + TITLE_LENGTH + 5, column);
    Lcd_Out(x, y + TITLE_LENGTH + 6, timeStr(t->mn));
}

void nextEditFeed1(int mode, int index, int x, int y)
{
    Time *t;
    t = &feedTime[index];
    if(mode == MODE_EDIT_FEED1_HH || mode == MODE_EDIT_FEED2_HH)
    {
        if(++t->hh >= 24)
        {
            t->hh = 0;
            t->mn = 0;
        }
    }
    else if( mode == MODE_EDIT_FEED1_MN || mode == MODE_EDIT_FEED2_MN)
    {
        if(++t->mn >= 60)
        {
            if(t->hh++ >= 24)
                t->hh = 0;
            t->mn = 0;
        }
    }
    displayEditFeed1(mode, index, x, y);
}

void previousEditFeed1(int mode, int index, int x, int y)
{
    Time *t;
    t = &feedTime[index];
    if(mode == MODE_EDIT_FEED1_HH || mode == MODE_EDIT_FEED2_HH)
    {
        if(--t->hh < 0)
        {
            t->hh = 23;
            t->mn = 0;
        }
    }
    else if(mode == MODE_EDIT_FEED1_MN || mode == MODE_EDIT_FEED2_MN)
    {
        if(--t->mn < 0)
        {
            if(--t->hh < 0)
                t->hh = 23;
            t->mn = 0;
        }
    }
    displayEditFeed1(mode, index, x, y);
}

/*
unsigned int testValues[] = {0x0730, 0x1230, 0x2115};
void initFeeder(unsigned int eepromAddress)
{
    unsigned int addr = BASE_ADDR;
    int n = 3;
    char *p = testValues;
    eepromWrite(eepromAddress, COUNTER_ADDR, 3);
    eepromWrite(eepromAddress, COUNTER_ADDR+1, 0);
    while(n != 0)
    {
        eepromWrite(eepromAddress, addr++ , p[1]);
        eepromWrite(eepromAddress, addr++ , p[0]);
        p+=2;
        n--;
    }
}

unsigned int readFeeder(unsigned int eepromAddress, unsigned char *values)
{
    unsigned char h;
    unsigned char l;
    unsigned int addr = BASE_ADDR;
    int count;
    int n;
    l = eepromRead(eepromAddress, COUNTER_ADDR+0);
    h = eepromRead(eepromAddress, COUNTER_ADDR+1);
    n = count = (h<<8)+l;
    while(n != 0)
    {
        *values++ = eepromRead(eepromAddress, addr++);
        *values++ = eepromRead(eepromAddress, addr++);
        n--;
    }
    return count;
}
*/
