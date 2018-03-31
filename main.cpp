#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

static UDPSocket sock(50101);
//static char IP[] = "192.168.137.1";
static char IP[] = "127.0.0.1";
static int port = 50100;

#define PLPIN 24
#define INHPIN 25

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
  
  char dummy[] = "ABC";
  char msg[] = "Alo alo";
  sock.sendTo (msg, strlen(msg)+1, IP, port);

    if (!bcm2835_init())
    {
      printf("bcm2835_init failed. Are you running as root??\n");
      return 1;
    }

    if (!bcm2835_spi_begin())
    {
      printf("bcm2835_spi_begin failed. Are you running as root??\n");
      return 1;
    }
    
    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The default
    bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   // The default
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32768); // The default 65536
    bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE);                      // The default
    
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      // the default
    
init ();


    bcm2835_gpio_fsel(PLPIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(PLPIN, HIGH);
    bcm2835_gpio_fsel(INHPIN, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(INHPIN, HIGH);
    fprintf (stderr, "4\n");
     
     int i=0;
    while (1) {
      fprintf (stderr, "5f %d\n", i++);
      bcm2835_gpio_write(PLPIN, LOW);
      usleep (10);
      bcm2835_gpio_write(PLPIN, HIGH);
      usleep (10);
      bcm2835_gpio_write(INHPIN, LOW);
      usleep (10);

      bcm2835_spi_transfernb (dummy, msg, 1);

      usleep (10);

      bcm2835_gpio_write(INHPIN, HIGH);

      fprintf (stderr, "%s ", binary (msg[0]));

      bcm2835_delay (1000);
      // n = sock.recv (buf, MAXBUF); 
      // if (n > 0) {
      //   sscanf (buf, "%s %s %d", type, device, &value);
      //   if (strcmp (type, "SO") == 0) {
      //     Device *p = sim.sevenSegments.find (device);
      //     ((Sevenseg *) p)->setInt (value);
      //   } else if (strcmp (type, "BO") == 0) {
      //     Device *p = sim.bitOuts.find(device);
      //     ((Bit *) p)->setValue (value);
      //   }
      // }
      // sim.registers7219.out();
      // sim.bitOuts.out();
    }
    bcm2835_spi_end();
    bcm2835_close();

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
