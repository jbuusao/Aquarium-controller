extern  void eepromWrite(int deviceAddress, unsigned int memAddress, unsigned char value);
extern  void eepromWriteWord(int deviceAddress, unsigned int memAddress, unsigned int word);
extern  void eepromWriteMulti(int deviceAddress, unsigned int memAddress, unsigned char *values, unsigned int size);
extern  unsigned char eepromRead(int deviceAddress, unsigned int memAddress);
extern  unsigned int eepromReadWord(int deviceAddress, unsigned int memAddress);
extern  void eepromReadMulti(int deviceAddress, unsigned int memAddress, unsigned char *values, unsigned int size);

