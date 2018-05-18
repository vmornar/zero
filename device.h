#ifndef __DEVICE
#define __DEVICE
#include <stdio.h>
#include <string>
#include <unistd.h>
using namespace std;

#ifdef emu
    #define BCM2835_H  // to skip it
    #include "bcm2835emu.h"
#else
    #include "bcm2835.h"
#endif

#define pinOut(pin,state) bcm2835_gpio_write((pin),(state))
#define pinGet(pin) bcm2835_gpio_lev((pin))
#define pinSet(pin) bcm2835_gpio_set((pin))
#define pinClr(pin) bcm2835_gpio_clr((pin))

class DeviceCollection;
class Device {
public:
    Device () {}
    string name;
    DeviceCollection *dc;
    int contentIndex;
    virtual void allocateBuf(DeviceCollection *dc);
};

class DeviceCollection {
private:
    Device **p = NULL;
public:
    int pin;
    unsigned char *buffer = NULL;
    unsigned char *oldBuffer = NULL; 
    int count = 0;
    int itemSize;
    Device * find (string name);
    void add (Device *d);
    Device * operator[] (int i);
    void out ();
    void in ();
};

class ShiftReg : public Device {
};

class Bit : public Device {
public:
    ShiftReg * shiftReg;
    int bit;
    void allocateBuf (DeviceCollection *dc) {}
    int getValue () {
        return shiftReg->dc->buffer[shiftReg->contentIndex] >> bit & 1;
    }
    int setValue(int value) {
        shiftReg->dc->buffer[shiftReg->contentIndex] |= (value << bit);
    }
};

class Simulator {
public:
    DeviceCollection registers7219;
    DeviceCollection sevenSegments;
    DeviceCollection shiftIns;
    DeviceCollection shiftOuts;
    DeviceCollection bitIns;
    DeviceCollection bitOuts;
};

unsigned char reverse(unsigned char c);
char *binary(unsigned char c);

extern Simulator sim;

#endif