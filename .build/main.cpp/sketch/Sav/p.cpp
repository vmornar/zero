#if 0

#include "gpio.h"
int main(int argc, char **argv) {
  int g=26, rep;

  setup_io();

  INP_GPIO(g);  OUT_GPIO(g);
  for (rep = 0; rep < 10; rep++) {
    BIT1(g);   sleep(1);
    BIT0(g);   sleep(1);
  }
  return 0;
} // main

#else

#include "bcm2835.h"
#include <stdio.h>

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

#endif