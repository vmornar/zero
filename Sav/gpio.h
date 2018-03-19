#define BCM2708_PERI_BASE 0x20000000 // zero and pi 1
//#define BCM2708_PERI_BASE        0x3F000000 // pi 2 & 3
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE_SIZE (4 * 1024)
#define BLOCK_SIZE (4 * 1024)

extern int mem_fd;
extern void *gpio_map;

// I/O access
extern volatile unsigned *gpio;

// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or
// SET_GPIO_ALT(x,y)
#ifdef emu
#define INP_GPIO(g) ;
#define OUT_GPIO(g) ;
#define GPIO_SET *gpio
#define GPIO_CLR *gpio
#define BIT1(g) fprintf (stderr, "%d 1 ", g);
#define BIT0(g) fprintf (stderr, "%d 0 ", g);
#else
#define INP_GPIO(g) *(gpio + ((g) / 10)) &= ~(7 << (((g) % 10) * 3))
#define OUT_GPIO(g) *(gpio + ((g) / 10)) |= (1 << (((g) % 10) * 3))
#define SET_GPIO_ALT(g, a)                                                     \
  *(gpio + (((g) / 10))) |=                                                    \
      (((a) <= 3 ? (a) + 4 : (a) == 4 ? 3 : 2) << (((g) % 10) * 3))

#define GPIO_SET *(gpio + 7) // sets   bits which are 1 ignores bits which are 0
#define GPIO_CLR                                                               \
  *(gpio + 10) // clears bits which are 1 ignores bits which are 0

#define GET_GPIO(g) (*(gpio + 13) & (1 << g)) // 0 if LOW, (1<<g) if HIGH

#define GPIO_PULL *(gpio + 37)     // Pull up/pull down
#define GPIO_PULLCLK0 *(gpio + 38) // Pull up/pull down clock
#define BIT1(g) GPIO_SET = 1 << g
#define BIT0(g) GPIO_CLR = 1 << g;
#endif
void setup_io();
