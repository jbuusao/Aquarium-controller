#include    "temperature.h"


char *temp_text = "+000.0000";
char text[4];

unsigned char lastValueL    = 0xff;
unsigned char lastValueH    = 0xff;
unsigned char lastTempL     = 0xff;
unsigned char lastTempH     = 0xff;
unsigned char lastConfig    = 0xff;
unsigned char i0, i1, i2, i3, i4, i5, i6, i7;


char *hex = "0123456789ABCDEF";

void displayByte(unsigned int x, unsigned int y, unsigned int value)
{
    text[0] = hex[(value&0xf0)>>4];
    text[1] = hex[value&0xf];
    text[2] = 0;
    Lcd_Out(x, y, text);
}

void displayWord(unsigned int x, unsigned int y, unsigned int value)
{
    unsigned hi, lo;
    hi = value >> 8;
    lo = value & 0xff;
    text[0] = hex[(hi&0xf0)>>4];
    text[1] = hex[hi&0xf];
    text[2] = hex[(lo&0xf0)>>4];
    text[3] = hex[lo&0xf];
    text[4] = 0;
    Lcd_Out(x, y, text);
}

void setCustomAlarm(int min, int max)
{
    getTemperature();
    min /= 5;
    max /= 5;
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&OW_PORT, OW_BIT, 0x4e);                   // Issue command WRITE_SCRATCHPAD
    Ow_Write(&OW_PORT, OW_BIT, (unsigned char)min);
    Ow_Write(&OW_PORT, OW_BIT, (unsigned char)max);
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&OW_PORT, OW_BIT, 0x48);                   // Issue command COPY_SCRATCHPAD
}

void getCustomAlarm(int *min, int *max)
{
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xcc);
    Ow_Write(&OW_PORT, OW_BIT, 0xbe);
    i0 = Ow_Read(&OW_PORT, OW_BIT);
    i1 = Ow_Read(&OW_PORT, OW_BIT);
    i2 = Ow_Read(&OW_PORT, OW_BIT);
    i3 = Ow_Read(&OW_PORT, OW_BIT);
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal

    *min = i2*5;
    *max = i3*5;
}

// Translate1: 23.45 -> 234
int translateToCustom(int temp2write)
{
    const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
    int sign = 1;
    char temp_whole, cc, dd, uu, c;
    unsigned int temp_fraction;

    if (temp2write & 0x8000)
      sign = -1;
    // Extract temp_whole
    temp_whole = temp2write >> RES_SHIFT ;
    // Convert temp_whole to characters
    if (temp_whole/100)
        cc = temp_whole/100;
    else
        cc = 0;
    dd = (temp_whole/10)%10;
    uu = temp_whole%10;
    temp_fraction  = temp2write << (4-RES_SHIFT);
    temp_fraction &= 0x000F;
    temp_fraction *= 625;
    c = temp_fraction/1000;
    return 1000*cc + 100*dd + 10*uu + c;
}

int testTemperature(int temperature, int minCustom, int maxCustom)
{
    temperature = translateToCustom(temperature);
    if(temperature < minCustom)
        return -1;
    if(temperature > maxCustom)
        return +1;
    return 0;
}

void displayID(unsigned int x, unsigned int y)
{
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0x33);
    i0 = Ow_Read(&OW_PORT, OW_BIT);
    i1 = Ow_Read(&OW_PORT, OW_BIT);
    i2 = Ow_Read(&OW_PORT, OW_BIT);
    i3 = Ow_Read(&OW_PORT, OW_BIT);
    i4 = Ow_Read(&OW_PORT, OW_BIT);
    i5 = Ow_Read(&OW_PORT, OW_BIT);
    i6 = Ow_Read(&OW_PORT, OW_BIT);
    i7 = Ow_Read(&OW_PORT, OW_BIT);

    displayByte(2, 1, i0);
    displayByte(2, 3, i1);
    displayByte(2, 5, i2);
    displayByte(2, 7,  i3);
    displayByte(2, 9, i4);
    displayByte(2, 11, i5);
    displayByte(2, 13, i6);
    displayByte(2, 15, i7);
}

int testAlarm()
{
    Ow_Reset(&OW_PORT, OW_BIT);                     // Reset
    Ow_Write(&OW_PORT, OW_BIT, 0xec);               // ALARM_SEARCH
    i0 = Ow_Read(&OW_PORT, OW_BIT);
    Ow_Reset(&OW_PORT, OW_BIT);                     // Reset
    return (i0 & 1) != ((i0 >> 1) & 1);
}

void displayTemperatureScratchpad(int x, int y)
{
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xcc);
    Ow_Write(&OW_PORT, OW_BIT, 0xbe);
    i0 = Ow_Read(&OW_PORT, OW_BIT);
    i1 = Ow_Read(&OW_PORT, OW_BIT);
    i2 = Ow_Read(&OW_PORT, OW_BIT);
    i3 = Ow_Read(&OW_PORT, OW_BIT);
    i4 = Ow_Read(&OW_PORT, OW_BIT);
    i5 = Ow_Read(&OW_PORT, OW_BIT);
    i6 = Ow_Read(&OW_PORT, OW_BIT);
    i7 = Ow_Read(&OW_PORT, OW_BIT);

    displayByte(x, y+0, i0);
    displayByte(x, y+2, i1);
    displayByte(x, y+4, i2);
    displayByte(x, y+6,  i3);
    displayByte(x, y+8, i4);
    displayByte(x, y+10, i5);
    displayByte(x, y+12, i6);
    displayByte(x, y+14, i7);
}

void formatTemperature(unsigned int temp2write)
{
    const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
    char temp_whole;
    unsigned int temp_fraction;

    // Check if temperature is negative
    if (temp2write & 0x8000) {
      temp_text[0] = '-';
      temp2write = ~temp2write + 1;
    }
    else
      temp_text[0] = '+';

    // Extract temp_whole
    temp_whole = temp2write >> RES_SHIFT ;

    // Convert temp_whole to characters
    if (temp_whole/100)
    temp_text[1] = temp_whole/100  + 48;
    else
    temp_text[1] = ' ';

    temp_text[2] = (temp_whole/10)%10 + 48;             // Extract tens digit
    temp_text[3] =  temp_whole%10     + 48;             // Extract ones digit
    temp_text[4] =  '.';

    // Extract temp_fraction and convert it to unsigned int
    temp_fraction  = temp2write << (4-RES_SHIFT);
    temp_fraction &= 0x000F;
    temp_fraction *= 625;

    // Convert temp_fraction to characters
    temp_text[5] =  temp_fraction/1000    + 48;         // Extract thousands digit
    temp_text[6] =  0;
}

void displayTemperature(int x, int y, unsigned int temp2write)
{
    formatTemperature(temp2write);
    Lcd_Out(x, y, temp_text);
}

unsigned int getTemperature()
{
    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal

    Ow_Write(&OW_PORT, OW_BIT, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&OW_PORT, OW_BIT, 0x44);                   // Issue command CONVERT_T
    Delay_us(200);

    Ow_Reset(&OW_PORT, OW_BIT);
    Ow_Write(&OW_PORT, OW_BIT, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&OW_PORT, OW_BIT, 0xBE);                   // Issue command READ_SCRATCHPAD

    lastValueL  = Ow_Read(&OW_PORT, OW_BIT);
    lastValueH  = Ow_Read(&OW_PORT, OW_BIT);
    lastTempH   = Ow_Read(&OW_PORT, OW_BIT);
    lastTempL   = Ow_Read(&OW_PORT, OW_BIT);
    lastConfig  = Ow_Read(&OW_PORT, OW_BIT);

    return (lastValueH<<8)+lastValueL;
}

char *getTemperatureAlarm1()
{
    const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
    unsigned char i0, i1, i2, i3;
    unsigned int alarmTemp;

    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xcc);
    Ow_Write(&OW_PORT, OW_BIT, 0xbe);
    i0 = Ow_Read(&OW_PORT, OW_BIT);
    i1 = Ow_Read(&OW_PORT, OW_BIT);
    i2 = Ow_Read(&OW_PORT, OW_BIT);
    i3 = Ow_Read(&OW_PORT, OW_BIT);
    alarmTemp = i3 << RES_SHIFT;
    formatTemperature(alarmTemp);
    return temp_text;
}

void setTemperatureAlarm()
{
    const unsigned short RES_SHIFT = TEMP_RESOLUTION - 8;
    unsigned int lastTemp;
    unsigned char value;

    getTemperature();
    lastTemp = (lastValueH<<8)+lastValueL;
    value = lastTemp >> RES_SHIFT;

    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&OW_PORT, OW_BIT, 0x4e);                   // Issue command WRITE_SCRATCHPAD
    Ow_Write(&OW_PORT, OW_BIT, lastTempH);
    Ow_Write(&OW_PORT, OW_BIT, value);
    Ow_Write(&OW_PORT, OW_BIT, lastConfig);

    Ow_Reset(&OW_PORT, OW_BIT);                         // Onewire reset signal
    Ow_Write(&OW_PORT, OW_BIT, 0xCC);                   // Issue command SKIP_ROM
    Ow_Write(&OW_PORT, OW_BIT, 0x48);                   // Issue command COPY_SCRATCHPAD
}
