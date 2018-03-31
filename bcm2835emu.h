#include <stdint.h>

#define HIGH 1
#define LOW 0
#define BCM2835_GPIO_FSEL_OUTP 0
#define BCM2835_SPI_CS_NONE 0
#define BCM2835_SPI_CLOCK_DIVIDER_1024 0
#define BCM2835_SPI_BIT_ORDER_MSBFIRST 0
#define BCM2835_SPI_MODE0 0
bool bcm2835_init();
void bcm2835_gpio_fsel(int pin, int mode);
void bcm2835_gpio_write(int pin, int val);
void bcm2835_delay(unsigned int ms);
void bcm2835_st_delay(unsigned int offset, unsigned int ms);
void bcm2835_close();
void bcm2835_aux_spi_write(uint16_t b);
void bcm2835_aux_spi_transfernb(unsigned char *out, unsigned char *in, int n);
void bcm2835_spi_transfernb(unsigned char *out, unsigned char *in, int n);
bool bcm2835_spi_begin ();
void bcm2835_spi_end();
void bcm2835_spi_chipSelect(int mode);
void bcm2835_spi_setBitOrder(int mode);      // The default
void bcm2835_spi_setDataMode(int mode);                   // The default
void bcm2835_spi_setClockDivider(int mode); // The default 65536
  