#include    "Aquarium6.h"
#include    "LCD.h"
#include    "RTC.h"
#include    "Feeder.h"
#include    "FeederServo.h"
#include    "Temperature.h"
#include    "EEPROM_24LC256.h"

int minTemp = 0;
int maxTemp = 0;
int temperature = 0;
int testResult = 0;

int mode = 0;
unsigned char state = 0;
unsigned char lastState = 0;
unsigned timerCounter;
unsigned pulseCounter;

unsigned int steps = 0;
//unsigned int rawData[] = {0, 0, 0};
//unsigned char stepData[] = "00:00 00:00 00:00 ";
//char *msgFeedTime = "G";

void interrupt() {
  if (TMR0IF_bit) {
    timerCounter++;
    pulseCounter++;

    TMR0IF_bit = 0; // re-arm interruptions
    TMR0   = 218;
  }
}

char *toStr(int value)
{
    if(value/100)
        temp_text[0] = value/100  + 48;
    else
        temp_text[0] = '0';
    temp_text[1] = (value/10)%10 + 48;             // Extract tens digit
    temp_text[2] = '.';
    temp_text[3] =  value%10     + 48;             // Extract ones digit
    temp_text[4] =  0;
    return temp_text;
}

void displayEdited(int x, int y)
{
    if(mode == MODE_EDIT_MIN)
    {
        Lcd_Out(x, y, "min=");
        Lcd_Out(x, y+4, toStr(minTemp));
        Lcd_Out(x, y+8, "       ");
    }
    else if(mode == MODE_EDIT_MAX)
    {
        Lcd_Out(x, y, "max=");
        Lcd_Out(x, y+4, toStr(maxTemp));
        Lcd_Out(x, y+8, "       ");
    }
    else
    {
        Lcd_Out(x, y, "Lim  ");
        Lcd_Out(x, y+5, toStr(minTemp));
        Lcd_Out(x, y+9, " ");
        Lcd_Out(x, y+11, toStr(maxTemp));
    }
}

void nextEdited(int x, int y)
{
    int *value;
    value = mode == MODE_EDIT_MIN? &minTemp : &maxTemp;
    *value += 5;
    if(*value >= 900)
        *value = 0;
    displayEdited(x, y);
}
void previousEdited(int x, int y)
{
    int *value;
    value = mode == MODE_EDIT_MIN? &minTemp : &maxTemp;
    *value -= 5;
    if(*value < 0)
        *value = 900;
    displayEdited(x, y);
}

void handleOKButton(int state)
{
    switch(mode)
    {
        case MODE_FEED:
            mode = MODE_EDIT_FEED1_HH;
            displayEditFeed1(mode, 0, 2, 1);
            break;
        case MODE_EDIT_FEED1_HH:
            mode = MODE_EDIT_FEED1_MN;
            displayEditFeed1(mode, 0, 2, 1);
            break;
        case MODE_EDIT_FEED1_MN:
            mode = MODE_EDIT_FEED2_HH;
            displayEditFeed1(mode, 1, 2, 1);
            break;
        case MODE_EDIT_FEED2_HH:
            mode = MODE_EDIT_FEED2_MN;
            displayEditFeed1(mode, 1, 2, 1);
            break;
        case MODE_EDIT_FEED2_MN:
            mode = MODE_FEED;
            saveFeederData();
            displayFeedTimes(2, 1);
            break;
        case MODE_SET_TEMP:
            if(minTemp > maxTemp)
                minTemp = maxTemp;
            mode = MODE_EDIT_MIN;
            displayEdited(2, 1);
            break;
        case MODE_EDIT_MIN:
            if(minTemp > maxTemp)
                maxTemp = minTemp;
            mode = MODE_EDIT_MAX;
            displayEdited(2, 1);
            break;
        case MODE_EDIT_MAX:
            mode = MODE_SET_TEMP;
            setCustomAlarm(minTemp, maxTemp);
            displayEdited(2, 1);
            break;
        case MODE_TEST_FEED:
          feed();
          break;
    }
}

void handleCounterEvent()
{
    Time *time = 0;
    timerCounter = 0;
    temperature = getTemperature();
    displayTemperature(1, 10, temperature);
    testResult = testTemperature(temperature, minTemp, maxTemp);
    
    time = RTC_displayTime(1, 1);
    if(Feeder_test(time))
    {
        //Lcd_Out(1, 1, msgFeedTime);
        feed();
        //Lcd_Out(1, 1, "               ");
    }

    if(testResult < 0)
    {
        ALARM_COLD_TRIS &= ~ALARM_COLD_BIT;
        ALARM_COLD_PORT |= ALARM_COLD_BIT;
    }
    else
    {
        ALARM_COLD_TRIS &= ~ALARM_COLD_BIT;
        ALARM_COLD_PORT &= ~ALARM_COLD_BIT;
    }
    if(testResult > 0)
    {
        ALARM_WARM_TRIS &= ~ALARM_WARM_BIT;
        ALARM_WARM_PORT |= ALARM_WARM_BIT;
    }
    else
    {
        ALARM_WARM_TRIS &= ~ALARM_WARM_BIT;
        ALARM_WARM_PORT &= ~ALARM_WARM_BIT;
    }
}

void displayMode(int x, int y)
{
    clearLine(x);
    switch(mode)
    {
        case MODE_SET_TEMP:
        case MODE_EDIT_MIN:
        case MODE_EDIT_MAX:
            displayEdited(x, y);
            break;
        case MODE_FEED:
            displayFeedTimes(x, y);
            break;
    }
}
void main()
{
    Time *time;
    ADCON0 = 0;
    ADCON1 = 0x06;              // All ports are digital

    I2C1_Init(100000);          // initialize I2C

    PORTB = 0xFF;
    TRISB = 0;

    // Comment following line for debugging
    OPTION_REG = 0x40;        // Assign prescaler to TMR0 (1/2)
    TMR0  = 12;              // Timer0 initial value
    INTCON = 0xA0;           // Enable TMRO interrupt

    ALARM_COLD_TRIS = 0;

    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Out(1, 1, "Started!");
    PORTC = 0;

    turnOffFeeder();
    loadFeederData();
    getCustomAlarm(&minTemp, &maxTemp);
    displayEdited(2, 1);

    while(1)
    {
        TRISC = TRISC|BUTTONS_MASK;
        state = PORTC & BUTTONS_MASK;

        if(state != lastState)
        {
          lastState = state;
          if(state & (1<<BUTTON_PREVIOUS))
          {
            if(mode == MODE_EDIT_FEED1_HH || mode == MODE_EDIT_FEED1_MN)
                previousEditFeed1(mode, 0, 2, 1);
            else if(mode == MODE_EDIT_FEED2_HH || mode == MODE_EDIT_FEED2_MN)
                previousEditFeed1(mode, 1, 2, 1);
            else if(mode == MODE_EDIT_MIN || mode == MODE_EDIT_MAX)
                previousEdited(2, 1);
            else
            {
                if(--mode < 0)
                    mode = MODE_COUNT -1;
                displayMode(2, 1);
            }
          }
          else if(state & (1<<BUTTON_NEXT))
          {
            if(mode == MODE_EDIT_FEED1_HH || mode == MODE_EDIT_FEED1_MN)
                nextEditFeed1(mode, 0, 2, 1);
            else if(mode == MODE_EDIT_FEED2_HH || mode == MODE_EDIT_FEED2_MN)
                nextEditFeed1(mode, 1, 2, 1);
            else if(mode == MODE_EDIT_MIN || mode == MODE_EDIT_MAX)
                nextEdited(2, 1);
            else
            {
                if(++mode >= MODE_COUNT)
                    mode = 0;
                displayMode(2, 1);
            }
          }

          if(state & (1<<BUTTON_OK))
            handleOKButton(state);
        }

        if (timerCounter >= 20000)
            handleCounterEvent();
    }
}
