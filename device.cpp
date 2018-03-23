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

void DeviceCollection::out () {
    if (memcmp(buffer, oldBuffer, count*itemSize) != 0) {
        #ifdef emu
            printf ("%p ", buffer);
        #endif
        bcm2835_gpio_write(pin, LOW);
        bcm2835_aux_spi_transfernb (buffer, NULL, count * itemSize);
        bcm2835_gpio_write(pin, HIGH);
        memcpy(oldBuffer, buffer, count*itemSize);
        #ifdef emu
            printf ("\n");
        #endif   
    }
}

void DeviceCollection::in () {
    bcm2835_aux_spi_transfernb (NULL, buffer, count * itemSize);
}

unsigned char reverse (unsigned char c) {
    unsigned char r = 0;
    for (int i = 0; i < 8; i++) {
        r |= ((c >> i) & 1) << (7-i);
    }
}

char *binary (unsigned char c) {
    static char b[8+1];
    for (int i = 7; i >= 0; i--) {
        b[7-i] = 48 + ((c >> i) & 1);
    }
    b[8] = 0;
    return b;
}
