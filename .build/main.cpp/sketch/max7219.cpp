#include <malloc.h>
#include "max7219.h"
#include "sevenseg.h"

#ifdef emu
    #define BCM2835_H  // to skip it
    #include "bcm2835emu.h"
#else
    #include "bcm2835.h"
#endif

void Max7219::setContent (const unsigned char *buf, int start, int digits) {
    int j = start;
    for (int i = 0; i < digits; i++) {
        if (buf[i] == '.') {
            dc->buffer[contentIndex*dc->itemSize + j - 1] |= 0b1000000;
        } else if (buf[i] > 127) {
             dc->buffer[contentIndex*dc->itemSize + j++] = Sevenseg::segments[(buf[i] & 0b01111111) - 32] | 0b1000000;
        } else {
            dc->buffer[contentIndex*dc->itemSize + j++] = Sevenseg::segments[buf[i]-32];
        }
    }
}

void Max7219::allocateBuf (DeviceCollection *dc) {
    dc->buffer = (unsigned char *) realloc (dc->buffer, dc->itemSize*dc->count);
    dc->oldBuffer = (unsigned char *) realloc (dc->oldBuffer, dc->itemSize*dc->count);
    this->dc = dc;
    contentIndex = dc->count - 1;
    for (int i = 0; i < dc->itemSize; i++) {
        dc->buffer[contentIndex*dc->itemSize + i] = 0;
    }
}

