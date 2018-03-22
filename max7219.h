#include "device.h"
class Max7219 : public Device {
public:
    int commonAnode;
    Max7219 () {};
    unsigned char content[8];
};
