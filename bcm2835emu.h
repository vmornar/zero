
#define HIGH 1
#define LOW 0
#define BCM2835_GPIO_FSEL_OUTP 0

bool bcm2835_init();
void bcm2835_gpio_fsel(int pin, int mode);
void bcm2835_gpio_write(int pin, int val);
void bcm2835_delay(unsigned int ms);
void bcm2835_close();
