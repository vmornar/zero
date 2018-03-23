#include <stdio.h>
#include <string.h>
#include "main.h"

static UDPSocket sock(50101);
//static char IP[] = "192.168.137.1";
static char IP[] = "127.0.0.1";
static int port = 50100;

Simulator sim;
#define MAXBUF 128

int main(int argc, char **argv) {
  unsigned char buf[MAXBUF+1];
  char device[64+1], type[16+1];
  int value, n;
  // for (int i=0; segments[i] < 255; i++) {
  //     printf ("%2d %c 0b%s ", i+32, i+32, Sevenseg::binary(Sevenseg::segments[i]));
  //     printf ("0b%s\n", Sevenseg::binary(Sevenseg::reverse(Sevenseg::segments[i])));
  // }

  init ();
  
  char msg[] = "Alo alo";
  sock.sendTo (msg, strlen(msg)+1, IP, port);

  while (1) {
    n = sock.recv (buf, MAXBUF); 
    if (n > 0) {
      sscanf (buf, "%s %s %d", type, device, &value);
      if (strcmp (type, "SO") == 0) {
        Device *p = sim.sevenSegments.find (device);
        ((Sevenseg *) p)->setInt (value);
      } else if (strcmp (type, "BO") == 0) {
        Device *p = sim.bitOuts.find(device);
        ((Bit *) p)->setValue (value);
      }
    }
    sim.registers7219.out();
    sim.bitOuts.out();
  }

  return 0;

  // if (!bcm2835_init()) return 1;
  // bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
  // for (int i = 0; i < 10; i++) {
  //   bcm2835_gpio_write(PIN, HIGH);
  //   bcm2835_delay(200);
  //   bcm2835_gpio_write(PIN, LOW);
  //   bcm2835_delay(200);
  // }
  // bcm2835_close();
  // return 0;
}
