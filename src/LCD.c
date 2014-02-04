#include    "LCD.h"

void clearLine(int x)
{
    Lcd_out(x, 1, "                ");
}
