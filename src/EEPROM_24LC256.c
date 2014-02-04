#include    "EEPROM_24LC256.h"

void eepromWrite(int deviceAddress, unsigned int memAddress, unsigned char value)
{
    I2C1_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 0);
    I2C1_Wr(memAddress>>8);
    I2C1_Wr(memAddress&0xff);
    I2C1_Wr(value);
    I2C1_Stop();
    delay_ms(100);
}//~

void eepromWriteMulti(int deviceAddress, unsigned int memAddress, unsigned char*values, unsigned int size)
{
    /*
    I2C1_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 0);
    I2C1_Wr(memAddress>>8);
    I2C1_Wr(memAddress&0xff);
    I2C1_Repeated_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 1);
    do {
        I2C1_Wr(*values++);
    }
    while(--size);
    I2C1_Stop();
    */
    while(size--)
        eepromWrite(0, memAddress++, *values);
}

unsigned char eepromRead(int deviceAddress, unsigned int memAddress)
{
    unsigned char value;
    I2C1_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 0);
    I2C1_Wr(memAddress>>8);
    I2C1_Wr(memAddress&0xff);
    I2C1_Repeated_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 1);
    value = I2C1_Rd(0u);
    I2C1_Stop();
    return value;
}//~

void eepromReadMulti(int deviceAddress, unsigned int memAddress, unsigned char*values, unsigned int size)
{
    I2C1_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 0);
    I2C1_Wr(memAddress>>8);
    I2C1_Wr(memAddress&0xff);
    I2C1_Repeated_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 1);
    while(size--)
        *values++ = I2C1_Rd(size!=0);
    I2C1_Stop();
}

unsigned char eepromReadCurrent(int deviceAddress)
{
    unsigned char value;
    I2C1_Start();
    I2C1_Wr(0xa0 + (deviceAddress << 1) + 1);
    value = I2C1_Rd(1u);
    I2C1_Stop();
    return value;
}//~
