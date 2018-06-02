#ifndef __DEVICE

#define __DEVICE
#include <stdio.h>
#include <string>
#include <unordered_map>
#ifndef emu
#include <unistd.h>
#endif
using namespace std;

#ifdef emu
#define BCM2835_H // to skip it
#include "bcm2835emu.h"
#else
#include "bcm2835.h"
#endif

#define pinOut(pin, state) bcm2835_gpio_write((pin), (state))
#define pinGet(pin) bcm2835_gpio_lev((pin))
#define pinSet(pin) bcm2835_gpio_set((pin))
#define pinClr(pin) bcm2835_gpio_clr((pin))
#define inputPin(pin) bcm2835_gpio_fsel((pin), BCM2835_GPIO_FSEL_INPT);
#define outputPin(pin) bcm2835_gpio_fsel((pin), BCM2835_GPIO_FSEL_OUTP);

class DeviceCollection;

class Device {
public:
  Device() {}
  string name;
  DeviceCollection *dc;
  int contentIndex;
  virtual void allocateBuf(DeviceCollection *dc);
  virtual void setValue(int val) {}
  virtual int getValue() { return 0; }
  virtual string getStr() { return ""; }
  virtual void setStr(string s) {}
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
  int itemSize = 1;
  Device *find(string name);
  void add(Device *d);
  Device *operator[](int i);
  void out();
  void in();
  void debugIn();
  void debugOut();
};

class ShiftIn : public DeviceCollection {
public:
  int shldPin;
  int inhPin;
  void init();
  void in();
};

class ShiftOut : public DeviceCollection {
public:
  int latchPin;
  void init();
  void out();
};

#define MAX7219_DECODE_REG      (0x09)
#define MAX7219_INTENSITY_REG   (0x0A)
#define MAX7219_SCANLIMIT_REG   (0x0B)
#define MAX7219_SHUTDOWN_REG    (0X0C)
#define MAX7219_DISPLAYTEST_REG (0x0F)

class Shift7219 : public DeviceCollection {

public:
  int loadPin;
  void init();
  void out();
  void byteOut(unsigned char c);
  void command (unsigned char cmd, unsigned char v);
};

class ShiftReg : public Device {};

class Bit : public Device {
public:
  ShiftReg *shiftReg;
  int bit;
  void allocateBuf(DeviceCollection *dc) {}
  int getValue() {
    return shiftReg->dc->buffer[shiftReg->contentIndex] >> bit & 1;
  }
  void setValue(int value) {
    shiftReg->dc->buffer[shiftReg->contentIndex] |= (value << bit);
  }
};

class Simulator {
public:
  int clockPin;

  unordered_map<string, Device *> vars;

  Shift7219 registers7219;
  ShiftIn shiftIns;
  ShiftOut shiftOuts;

  Simulator() {
    shiftIns.id = 0;
    registers7219.id = 1;
    shiftOuts.id = 2;
  }

  DeviceCollection sevenSegments;
  DeviceCollection bitIns;
  DeviceCollection bitOuts;
};

unsigned char reverse(unsigned char c);
char *binary(unsigned char c);
unsigned char unBinary(char *s);

extern Simulator sim;

#endif