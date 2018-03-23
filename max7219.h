#include "device.h"

class Max7219 : public Device {
public:
    int commonAnode;
    Max7219 () {};
    void setContent (const char *buf, int start, int digits);
    void allocateBuf (DeviceCollection *dc);
};
