#include    "feederServo.h"
#include    "aquarium6.h"
#include    "lcd.h"

#define CLK_TRIS    TRISC
#define CLK_PORT    PORTC
#define CLK_BIT     2
#define REPEAT  20

extern char timeOn;
extern  unsigned pulseCounter;
extern  unsigned timerCounter;

int servoActive = 1;
int pulseState = LOW;
int pulseValue = PULSE_MAX;
int pulseWidth = PULSE_MIN;
int pulseRepeat = PULSE_REPEAT;
int pulseDelta = 0;

void pulseDuty()
{
    if(servoActive)
    {
      if(pulseState == HIGH && pulseCounter >= pulseValue)
      {
          pulseCounter = 0;
          pulseState = LOW;
          CLK_TRIS &= ~(1<<CLK_BIT);
          CLK_PORT = 0;
          if(pulseRepeat-- <= 0)
          {
            pulseRepeat = PULSE_REPEAT;
            pulseValue += pulseDelta;
          }
      }
      else if(pulseState == LOW && pulseCounter >= PULSE_IDLE)
      {
          pulseCounter = 0;
          pulseState = HIGH;
          CLK_TRIS &= ~(1<<CLK_BIT);
          CLK_PORT = 1<<CLK_BIT;
      }
    }
}

void doPulseRepeat(int repeat)
{
    if(servoActive)
    {
      pulseRepeat = repeat;
      while(1)
      {
        if(pulseState == HIGH && pulseCounter >= pulseValue)
        {
            pulseCounter = 0;
            pulseState = LOW;
            CLK_TRIS &= ~(1<<CLK_BIT);
            CLK_PORT = 0;
            if(pulseRepeat-- <= 0)
            {
              pulseRepeat = PULSE_REPEAT;
              return;
            }
        }
        else if(pulseState == LOW && pulseCounter >= PULSE_IDLE)
        {
            pulseCounter = 0;
            pulseState = HIGH;
            CLK_TRIS &= ~(1<<CLK_BIT);
            CLK_PORT = 1<<CLK_BIT;
        }
      }
    }
}

void turnOnFeeder()
{
    servoActive = 1;
    pulseValue = PULSE_MIN;
}

void turnOffFeeder()
{
    servoActive = 1;
    pulseValue = PULSE_MIN;
    doPulseRepeat(10);
    servoActive = 0;
}

void feed()
{
    turnOnFeeder();
    pulseValue = PULSE_MIN;
    pulseDelta = 1;

    while(pulseValue <= PULSE_MAX)
    {
        pulseDuty();
        pulseDelta = PULSE_INCREMENT;
    }
    doPulseRepeat(30);
    while(pulseValue >= PULSE_MIN)
    {
        pulseDuty();
        pulseDelta = -PULSE_INCREMENT;
    }
    doPulseRepeat(30);
    while(pulseValue <= PULSE_MAX)
    {
        pulseDuty();
        pulseDelta = PULSE_INCREMENT;
    }
    doPulseRepeat(30);
    while(pulseValue >= PULSE_MIN)
    {
        pulseDuty();
        pulseDelta = -PULSE_INCREMENT;
    }
    doPulseRepeat(40);
    turnOffFeeder();
}
