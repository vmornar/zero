#include "device.h"
#define ss 0

extern unsigned char segments[];

class Sevenseg : public Device {
public:
  Max7219 *max7219;
  int digits;
  int start;
  int value;
  string display;
  string format;
  Sevenseg() {};
  void init();
  void setValue(int val);
  void setStr(string s);
  void setStr(char *s);
  int getValue();
  // string getStr();
  static unsigned char segments[];
  void allocateBuf (DeviceCollection *dc) {}
};