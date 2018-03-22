#include "main.h"

static UDPSocket sock(50101);
static char IP[] = "192.168.137.1";
static int port = 50100;

simulator sim;

int main(int argc, char **argv) {

  init ();
  return;


  for (int i=0; segments[i] < 255; i++) {
      printf ("%2d 0b%s ", i+32, Sevenseg::binary(segments[i]));
      printf ("0b%s\n", Sevenseg::binary(Sevenseg::reverse(segments[i])));
  }

  return;
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
