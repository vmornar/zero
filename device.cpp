#include <malloc.h>
#include <string.h>
#include "device.h"

void Device::allocateBuf (DeviceCollection *dc) {
    dc->buffer = (unsigned char *) realloc (dc->buffer, dc->count);
    dc->oldBuffer = (unsigned char *) realloc (dc->oldBuffer, dc->count);
    this->dc = dc;
    contentIndex = dc->count - 1;
    dc->buffer[contentIndex] = 0;
    dc->oldBuffer[contentIndex] = 0;
}

// DeviceCollection::DeviceCollection (int id) {
//     this->id = id;
// }

Device * DeviceCollection::find (string name) {
    for (int i=0; i < count; i++) {
        if (name == p[i]->name) return p[i];
    }
    return NULL;
}

void DeviceCollection::add (Device *d) {
    p = (Device **) realloc(p, sizeof (Device *) * (++count));
    p[count-1] = d;
    d->allocateBuf (this);
}

Device * DeviceCollection::operator[] (int i) {
    return p[i]; 
}

void DeviceCollection::debugIn () {
    FILE *f = fopen ("in.txt", "r");
    char buf[512];
    int i=0;
    while (fscanf (f, "%s", buf) > 0) {
        buffer[i++] = unBinary (buf);
    }
    fclose (f);
}

void DeviceCollection::debugOut() {
    FILE *f;
    if (id == 1) f = fopen ("1.txt", "w");
    else f= fopen ("2.txt", "w");
    for (int i=0; i < count; i++) {
        fprintf ("%s ", binary(buffer[i]));
    }
    fclose (f);
}

// void DeviceCollection::out () {
//     if (memcmp(buffer, oldBuffer, count*itemSize) != 0) {
//         #ifdef emu
//             printf ("%p ", buffer);
//         #endif
//         bcm2835_gpio_write(pin, LOW);
//         bcm2835_aux_spi_transfernb ((char *) buffer, NULL, count * itemSize);
//         bcm2835_gpio_write(pin, HIGH);
//         memcpy(oldBuffer, buffer, count*itemSize);
//         #ifdef emu
//             printf ("\n");
//         #endif   
//     }
// }

// void DeviceCollection::in () {
//     bcm2835_aux_spi_transfernb (NULL, (char *) buffer, count * itemSize);
// }

ShiftIn::ShiftIn (int id) {
    this->id = id;
}

void ShiftIn::init () {
    inputPin (dataPin);
    inputPin (inhPin);
    inputPin (shldPin);
    pinSet (inhPin);
    pinSet (shldPin);
}

void ShiftIn::in () {
    int i, j;
    unsigned char b;
    pinClr (shldPin);
    delayMicroseconds (10);
    pinSet (shldPin);
    pinClr (inhPin);
    for (i=0; i < count; i++) {
        b = 0;
        for (j=0; j < 8; j++) {
            pinSet (sim.clockPin);
            delayMicroseconds (5);
            b |= (pinGet(dataPin) >> j);
            pinClr (sim.clockPin);
            delayMicroseconds (5);
        }
        buffer[i] = b;
    }
    pinSet (inhPin);
}


ShiftOut::ShiftOut (int id) {
    this->id = id;
}

void ShiftOut::init () {
    outputPin (dataPin);
    outputPin (latchPin);
    pinClr (latchPin);
}

void ShiftOut::out () {
    int i, j;
    unsigned char b;
    for (i=0; i < count; i++) {
        b = buffer[i];
        for (j=0; j < 8; j++) {
            pinClr (sim.clockPin);
            delayMicroseconds (5);
            pinOut (dataPin, b & (1 >> j));
            pinSet (sim.clockPin);
            delayMicroseconds (5);
        }
    }
    pinClr (sim.clockPin);
    pinSet (latchPin);
    delayMicroseconds (10);
    pinClr (latchPin);
}

Shift7219::Shift7219 (int id) {
    this->id = id;
}

void Shift7219::init () {
    outputPin (dataPin);
    outputPin (loadPin);
    pinClr (loadPin);
}

void Shift7219::out () {
    int i, j;
    unsigned char b;
    for (i=0; i < count; i++) {
        b = buffer[i];
        for (j=0; j < 8; j++) {
            pinClr (sim.clockPin);
            delayMicroseconds (5);
            pinOut (dataPin, b & (1 >> j));
            pinSet (sim.clockPin);
            delayMicroseconds (5);
        }
    }
    pinClr (sim.clockPin);
    pinSet (loadPin);
    delayMicroseconds (10);
    pinClr (loadPin);
}

unsigned char reverse (unsigned char c) {
    unsigned char r = 0;
    for (int i = 0; i < 8; i++) {
        r |= ((c >> i) & 1) << (7-i);
    }
    return r;
}

char *binary (unsigned char c) {
    static char b[8+1];
    for (int i = 7; i >= 0; i--) {
        b[7-i] = 48 + ((c >> i) & 1);
    }
    b[8] = 0;
    return b;
}

unsigned char unBinary (char *s) {
    int r=0, d;
    for (int i = 0; i < 8; i++) {
        d = s[i] - 48;
        r += d << 7-i;
    }
}