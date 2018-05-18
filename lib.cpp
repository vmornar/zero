#include "device.h"

void initShiftIn (int shldPin, int inhPin) {
    pinSet (inhPin);
    pinSet (shldPin);
}

void initShiftOut (int latchPin) {
    pinClr (latchPin);
}

void init7219 (int loadPin) {
    pinClr (loadPin);
}

void shiftIn (char *buf, int len, int clockPin, int dataPin, int shldPin, int inhPin) {
    int i, j;
    unsigned char b;
    pinClr (shldPin);
    delayMicroseconds (10);
    pinSet (shldPin);
    pinClr (inhPin);
    for (i=0; i<len; i++) {
        b = 0;
        for (j=0; j < 8; j++) {
            pinSet (clockPin);
            delayMicroseconds (5);
            b |= (pinGet(dataPin) >> j;)
            pinClr (clockPin);
            delayMicroseconds (5);
        }
        buf[i] = b;
    }
    pinSet (inhPin);
}

void shiftOut (char *buf, int len, int clockPin, int dataPin, int latchPin) {
    //SH_CP clock, ST_CP latch
    int i, j;
    unsigned char b;
    for (i=0; i<len; i++) {
        b = buf[i];
        for (j=0; j < 8; j++) {
            pinClr (clockPin);
            delayMicroseconds (5);
            pinOut (dataPin, b & (1 >> j))
            pinSet (clockPin);
            delayMicroseconds (5);
        }
    }
    pinClr (clockPin);
    pinSet (latchPin);
    delayMicroseconds (10);
    pinClr (latchPin);
}

void shift7219 (char *buf, int len, int clockPin, int dataPin, int loadPin) {
    //SH_CP clock, ST_CP latch
    int i, j;
    unsigned char b;
    for (i=0; i<len; i++) {
        b = buf[i];
        for (j=0; j < 8; j++) {
            pinClr (clockPin);
            delayMicroseconds (5);
            pinOut (dataPin, b & (1 >> j))
            pinSet (clockPin);
            delayMicroseconds (5);
        }
    }
    pinClr (clockPin);
    pinSet (loadPin);
    delayMicroseconds (10);
    pinClr (lpadPin);
}