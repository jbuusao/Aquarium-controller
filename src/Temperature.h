#define OW_PORT PORTA
#define OW_BIT  5

// DS1820  = 9 bits resolution
// DS18S20 = 9 bits resolution
// DS18B20 = 12 bits (default)

#define TEMP_RESOLUTION   9

extern  unsigned int getTemperature();
extern  void displayTemperature(int x, int y, unsigned int value);
extern  void displayTemperatureScratchpad(int x, int y);
extern  unsigned int getTemperature();
extern  void displayTemperature(int x, int y, unsigned int value);
extern  void displayTemperatureScratchpad(int x, int y);
extern  void setTemperatureAlarm();
extern  int int testAlarm();
extern  void displayID(unsigned int x, unsigned int y);
extern  char *getTemperatureAlarm1();
extern  void displayHex(unsigned int x, unsigned int y, unsigned int value);
extern  void getCustomAlarm(int *min, int *max);
extern  void setCustomAlarm(int min, int max);
extern  char *temperatureCustomToString(int byteValue);
extern  unsigned char temperatureCustomToByte(char *str);
extern  int testTemperature(int temperature, int minCustom, int maxCustom);


extern  char *hex;
extern  char *temp_text;
