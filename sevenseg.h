#include "device.h"
#define ss 0

extern unsigned char segments[];

class Sevenseg : public Device {
public:
    int registerIndex;
    int digits;
    int start;
    string content = "";
    string format;
    Sevenseg () {};
    void setInt (int val);
    void setStr (char *s);
    static unsigned char reverse (unsigned char c);
    static char *binary (unsigned char c);
};