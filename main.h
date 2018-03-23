#include <string>
#include <string.h>
using namespace std;
#include <stdio.h>
#include "sockets.h"
#include "max7219.h"
#include "sevenseg.h"
#include "device.h"

#ifdef emu
    #define BCM2835_H  // to skip it
    #include "bcm2835emu.h"
#else
    #include "bcm2835.h"
#endif

// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN 26 //RPI_V2_GPIO_P1_37

void init ();