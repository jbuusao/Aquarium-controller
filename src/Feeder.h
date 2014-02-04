#include    "Time.h"

extern  int Feeder_test(Time *time);
extern  void loadFeederData();
extern  void saveFeederData();
extern  void displayFeedTimes(int x, int y);
extern void initFeeder(unsigned eepromAddress);
extern  int feederIsOpen();
extern  void toggleFeeder();
extern unsigned int readFeeder(unsigned eppromAddress, unsigned char *values);
extern  void displayEditFeed1(int mode, int index, int x, int y);
extern  void nextEditFeed1(int mode, int index, int x, int y);
extern  void previousEditFeed1(int mode, int index, int x, int y);
