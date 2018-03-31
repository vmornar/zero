#ifdef emu

#include <stdio.h>
#include <unistd.h>
#include "bcm2835emu.h"
#include "device.h"

bool bcm2835_init() { 
    return true;
}

void bcm2835_gpio_fsel(int pin, int mode) {
    return;
}

void bcm2835_gpio_write(int pin, int val) {
    fprintf (stderr, "%d %d ", pin, val);
}

void bcm2835_delay(unsigned int ms) {
    usleep (ms*1000);
}

void bcm2835_st_delay(unsigned int offset, unsigned int ms) {
    usleep (ms);
}

void bcm2835_close() {
    getchar();
}

void bcm2835_aux_spi_write(uint16_t b) {
    printf ("%s ", binary(b));
}

void bcm2835_aux_spi_transfernb (unsigned char *output, unsigned char *input, int n) {
    if (output) {
        for (int i=0; i < n; i++) {
            printf ("%s ", binary(output[i]));
        }
    }    
}

void bcm2835_spi_transfernb (unsigned char *output, unsigned char *input, int n) {
    if (output) {
        for (int i=0; i < n; i++) {
            printf ("%s ", binary(output[i]));
        }
    }    
}

bool bcm2835_spi_begin () {return true;}
void bcm2835_spi_end() {}
void bcm2835_spi_chipSelect(int mode) {}
void bcm2835_spi_setBitOrder(int mode) {}      // The default
void bcm2835_spi_setDataMode(int mode) {}                  // The default
void bcm2835_spi_setClockDivider(int mode) {} // The default 65536
#endif