#include "device.h"
#define ss 0

extern unsigned char segments[];

class Sevenseg : public Device {
public:
  Max7219 *max7219;
  int digits;
  int start;
  string display = "";
  string format;
  Sevenseg(){};
  void setInt(int val);
  void setStr(char *s);
  static unsigned char segments[];
  void allocateBuf (DeviceCollection *dc) {}
};