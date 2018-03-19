#include <string.h>

#ifdef emu
#define BCM2835_H  // to skip it
#include "bcm2835emu.h"
#else
#include "bcm2835.h"
#endif
// Blinks on RPi Plug P1 pin 11 (which is GPIO pin 17)
#define PIN 26 //RPI_V2_GPIO_P1_37

#include "sockets.h"

static UDPSocket sock(50101);
static char IP[] = "192.168.137.1";
static int port = 50100;


int main(int argc, char **argv) {
  char msg[] = "Alo alo";
  sock.sendTo (msg, strlen(msg)+1, IP, port);

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
