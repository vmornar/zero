#include <string>
#include <string.h>
#include <stdlib.h>

using namespace std;

#include <stdio.h>
#include <unordered_map>

#include "sockets.h"

#ifdef emu
    #include <conio.h>
    #include <winbase.h>
    #define BCM2835_H  // to skip it
    #include "bcm2835emu.h"
#else
    #include "bcm2835.h"
#endif

#include "max7219.h"
#include "sevenseg.h"
#include "device.h"
// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN 26 //RPI_V2_GPIO_P1_37

void init ();