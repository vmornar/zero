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
#define inputPin(pin) bcm2835_gpio_fsel((pin), BCM2835_GPIO_FSEL_OUTP);
#define outputPin(pin) bcm2835_gpio_fsel((pin), BCM2835_GPIO_FSEL_INPT);

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
    // DeviceCollection(int id);
    int id;
    int dataPin;
    unsigned char *buffer = NULL;
    unsigned char *oldBuffer = NULL; 
    int count = 0;
    int itemSize = 8;
    Device * find (string name);
    void add (Device *d);
    Device * operator[] (int i);
    void out ();
    void in ();
    void debugIn();
    void debugOut();
};

class ShiftIn :public DeviceCollection {
public: 
    ShiftIn(int id);
    int shldPin;
    int inhPin;
    void init();
    void in ();
};

class ShiftOut : public DeviceCollection {
public: 
    ShiftOut(int id);
    int latchPin;
    void init();
    void out ();
};

class Shift7219 : public DeviceCollection {
public: 
    Shift7219 (int id);
    int loadPin;
    void init();
    void out ();
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
    int clockPin;
    Shift7219 registers7219(1);
    ShiftIn shiftIns(0);
    ShiftOut shiftOuts(2);

    DeviceCollection sevenSegments;
    DeviceCollection bitIns;
    DeviceCollection bitOuts;
};

unsigned char reverse(unsigned char c);
char *binary(unsigned char c);
unsigned unBinary(char *s);

extern Simulator sim;

#endif