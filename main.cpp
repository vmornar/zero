#include "main.h"
#include <stdio.h>
#include <string.h>

#ifndef emu
#include <unistd.h>
#endif

static UDPSocket sock(50100);

// static char IP[] = "192.168.137.1";
// static char IP[] = "127.0.0.1";
static char IP[] = "192.168.255.255";
static int port = 50101;

 static int pins[] = {
     14, // 8 inh, inh 165
     15, // 7 7seg in, data 7219
     18, // 6 miso_out, data 165, data in
     23, // 5 mosi_in, data 595, data out
     24, // 4 clk
     25, // 3 sh/ld 165
     8,  // 2 rck 595, latch
     7,   // 1 load MAX7219
     21
};

// static int pins[] = {
//     15, // 8 inh, inh 165
//     16, // 7 7seg in, data 7219
//     1,  // 6 miso_out, data 165
//     4,  // 5 mosi_in, data 595
//     5,  // 4 clk
//     6,  // 3 sh/ld 165
//     10, // 2 rck 595
//     11,  // 1 load MAX7219
//     29
// };

#define PLPIN 24
#define INHPIN 25

Simulator sim;
#define MAXBUF 128

int main(void) {
  char buf[MAXBUF + 1];
  char device[64 + 1], type[16 + 1];

  // for (int i=0; segments[i] < 255; i++) {
  //     printf ("%2d %c 0b%s ", i+32, i+32,
  //     Sevenseg::binary(Sevenseg::segments[i])); printf ("0b%s\n",
  //     Sevenseg::binary(Sevenseg::reverse(Sevenseg::segments[i])));
  // }

  // char dummy[] = "ABC";
  // char msg[] = "Alo alo";
  // sock.sendTo(msg, strlen(msg) + 1, IP, port);
  printf("Started 2\n");
  if (!bcm2835_init()) {
    printf("bcm2835_init failed. Are you running as root??\n");
    return 1;
  }

  // for (int i = 0; i < 9; i++) {
  //   bcm2835_gpio_fsel(pins[i], BCM2835_GPIO_FSEL_OUTP);
  // }

  // while (1) {
  //   for (int i=0; i < 9; i++) {
  //     pinSet(pins[i]);
  //     delayMicroseconds(100);
  //     pinClr(pins[i]);
  //   }
  // }

  // if (!bcm2835_spi_begin())
  // {
  //   printf("bcm2835_spi_begin failed. Are you running as root??\n");
  //   return 1;
  // }

  // bcm2835_spi_begin();
  // bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);      // The
  // default bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);                   //
  // The default bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32768);
  // // The default 65536 bcm2835_spi_chipSelect(BCM2835_SPI_CS_NONE); // The
  // default bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);      //
  // the default

  init();
  char cmd[16], name[64], cValue[64];
  string sName;
  int value = 0, n, simIndex;
  string sValue;
  Device *p;

  // while (1) {
  //   pinSet(sim.clockPin);
  //   waitMicroSec(1);
  //   // usleep(3);
  //   pinClr(sim.clockPin);
  //   waitMicroSec(1);
  //   // usleep(3);
  // }
  p = findVar("Vertical");
  p->setValue(12345);
  p = findVar("Altitude");
  p->setValue(67890);
  while (1) {
    // fprintf(stderr, "5f %d\n", i++);
    // bcm2835_gpio_write(PLPIN, LOW);
    // usleep(10);
    // bcm2835_gpio_write(PLPIN, HIGH);
    // usleep(10);
    // bcm2835_gpio_write(INHPIN, LOW);
    // usleep(10);
    // bcm2835_spi_transfernb(dummy, msg, 1);
    // usleep(10);
    // bcm2835_gpio_write(INHPIN, HIGH);
    // fprintf(stderr, "%s ", binary(msg[0]));
    // bcm2835_delay(1000);
    n = sock.recv(buf, MAXBUF);
    if (n > 0) {
      buf[n] = 0;
      printf("%s\n", buf);
      sscanf(buf, "%d %s", &simIndex, cValue);
      if (sim.devices[simIndex] != NULL) {
        sim.devices[simIndex]->setStr(cValue);
      }
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
    // sim.shiftOuts.out();

    sim.shiftIns.in();

    // Control print
    // for (int i=0; i < sim.shiftIns.count; i++) {
    //   printf ("%s %s ", sim.shiftIns[i]->name.c_str(),
    //   binary(sim.shiftIns.buffer[i]));
    // }
    // printf ("\n");

    // Did something change?
    if (memcmp(sim.shiftIns.buffer, sim.shiftIns.oldBuffer,
               sim.shiftIns.count) != 0) {

      for (int i = 0; i < sim.rotaryEncoders.count; i++) {
        int v = sim.rotaryEncoders[i]->getValue();
        if (v != 0) {
          printf("%s %d\n", sim.rotaryEncoders[i]->name.c_str(), v);
          sprintf(buf, "%d %d", sim.rotaryEncoders[i]->simIndex, v);
          sock.sendTo(buf, strlen(buf) + 1, IP, port);
        }
      }

      for (int i = 0; i < sim.bitIns.count; i++) {
        Bit *b = (Bit *)sim.bitIns[i];
        if (b->changed()) {
          printf("%s %d\n", b->name.c_str(), b->getValue());
          sprintf(buf, "%d %d", b->simIndex, b->getValue());
          sock.sendTo(buf, strlen(buf) + 1, IP, port);
        }
      }
      memcpy(sim.shiftIns.oldBuffer, sim.shiftIns.buffer, sim.shiftIns.count);
    }

    // Did something change in outputs?
    if (memcmp(sim.shiftOuts.buffer, sim.shiftOuts.oldBuffer,
               sim.shiftOuts.count) != 0) {
      memcpy(sim.shiftOuts.oldBuffer, sim.shiftOuts.buffer,
             sim.shiftOuts.count);
      sim.shiftOuts.out();
    }

    if (memcmp(sim.registers7219.buffer, sim.registers7219.oldBuffer,
               sim.registers7219.count * sim.registers7219.itemSize) != 0) {
      memcpy(sim.registers7219.oldBuffer, sim.registers7219.buffer,
             sim.registers7219.count * sim.registers7219.itemSize);
      sim.registers7219.out();
    }
    /*
        printf (">");
        scanf ("%s", cmd);
        if (strcmp(cmd, "sv") == 0) {
          scanf ("%s %d", name, &value);
          sName = name;
          p = sim.vars[sName];
          if (!p) printf ("Not found: %s\n", sName.c_str());
          else p->setValue (value);
        } else if (strcmp(cmd, "ss") == 0) {
          scanf ("%s %s", name, cValue);
          sName = name;
          sValue = cValue;
          p = sim.vars[sName];
          if (!p) printf ("Not found: %s\n", sName.c_str());
          else p->setStr (sValue);
        } else if (strcmp (cmd, "gv") == 0) {
          scanf ("%s", name);
          sName = name;
          p = sim.vars[sName];
          if (!p) printf ("Not found: %s\n", sName.c_str());
          else printf ("%d\n", p->getValue());
        } else if (strcmp (cmd, "gv") == 0) {
          scanf ("%s", name);
          sName = name;
          p = sim.vars[sName];
          if (!p) printf ("Not found: %s\n", sName.c_str());
          else printf ("%s\n", p->getStr().c_str());
        }
        sim.shiftIns.debugIn();
        sim.shiftOuts.debugOut();
        sim.registers7219.debugOut();
    */

    // getch();
  }

  // bcm2835_spi_end();
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
