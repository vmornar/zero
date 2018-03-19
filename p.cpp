#include <stdio.h>
#include "bcm2835.h"

// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN 26 //RPI_V2_GPIO_P1_37

int main(int argc, char **argv) {
  if (!bcm2835_init()) return 1;
  bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
  for (int i = 0; i < 10; i++) {
    bcm2835_gpio_write(PIN, HIGH);
    bcm2835_delay(200);
    bcm2835_gpio_write(PIN, LOW);
    bcm2835_delay(200);
  }
  bcm2835_close();
  return 0;
}
