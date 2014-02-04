#define CLK_TRIS    TRISC
#define CLK_PORT    PORTC
#define CLK_BIT     2

#define LOW     0
#define HIGH    1

#define VALUE_MAX   1024
#define VALUE_MIN   0
#define PULSE_MAX   60  // 42 theoric:50
#define PULSE_MIN   30  //30 theoric:25
#define PULSE_IDLE  406 // 290
#define PULSE_REPEAT    1
#define PULSE_INCREMENT    2

extern  int pulseState;
extern  int pulseValue;
extern  int servoActive;

extern  unsigned pulseCounter;
extern  unsigned timerCounter;

extern  void feed();
extern  void closeFeeder();
extern  void openFeeder();
extern  void pulseDuty();
extern  void zeroFeeder();
extern  void turnOnFeeder();
extern  void turnOffFeeder();
