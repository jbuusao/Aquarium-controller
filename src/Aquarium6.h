#define     BUTTON_OK       7
#define     BUTTON_PREVIOUS 6
#define     BUTTON_NEXT     5
#define     BUTTONS_MASK    ((1<<BUTTON_OK)|(1<<BUTTON_PREVIOUS)|(1<<BUTTON_NEXT))

#define     MODE_FEED       0
#define     MODE_TEST_FEED  1
#define     MODE_SET_TEMP   2
#define     MODE_COUNT      3
#define     MODE_EDIT_MIN   10
#define     MODE_EDIT_MAX   11

#define     MODE_EDIT_FEED  11
#define     MODE_EDIT_FEED1_HH  110
#define     MODE_EDIT_FEED1_MN  111
#define     MODE_EDIT_FEED2 21
#define     MODE_EDIT_FEED2_HH  210
#define     MODE_EDIT_FEED2_MN  211

#define     ALARM_COLD_TRIS TRISD
#define     ALARM_COLD_PORT PORTD
#define     ALARM_COLD_BIT  1
#define     ALARM_WARM_TRIS TRISD
#define     ALARM_WARM_PORT PORTD
#define     ALARM_WARM_BIT  2

#define     CLK_TRIS    TRISC
#define     CLK_PORT    PORTC
#define     CLK_BIT     2

#define     HANDLE_TEMPERATURE
#define     HANDLE_RTC

extern  char*   msgFeedTime;
