#ifdef emu
#include <stdio.h>
#include <unistd.h>
#include "bcm2835emu.h"

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
void bcm2835_close() {
    getchar();
    return;
}
#endif