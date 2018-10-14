#include "device.h"
#include <malloc.h>
#include <string.h>

#ifdef emu
#include "gettimeofday.h"
#include <winsock.h>
#else
#include <sys/time.h>
#endif

void waitMicroSec(int usec) {
  struct timeval t1, t2;
  long dif;

  usec *= 1;

  gettimeofday(&t1, NULL);
  do {
    gettimeofday(&t2, NULL);
    dif = t2.tv_sec * 1000000 + t2.tv_usec - (t1.tv_sec * 1000000 + t1.tv_usec);
  } while (dif < usec);
}

void Device::allocateBuf(DeviceCollection *dc) {
  dc->buffer = (unsigned char *)realloc(dc->buffer, dc->count);
  dc->oldBuffer = (unsigned char *)realloc(dc->oldBuffer, dc->count);
  this->dc = dc;
  contentIndex = dc->count - 1;
  dc->buffer[contentIndex] = 0;
  dc->oldBuffer[contentIndex] = 0;
}

// DeviceCollection::DeviceCollection (int id) {
//     this->id = id;
// }

Device *DeviceCollection::find(string name) {
  for (int i = 0; i < count; i++) {
    if (name == p[i]->name)
      return p[i];
  }
  return NULL;
}

void DeviceCollection::add(Device *d) {
  p = (Device **)realloc(p, sizeof(Device *) * (++count));
  p[count - 1] = d;
  d->allocateBuf(this);
}

Device *DeviceCollection::operator[](int i) { return p[i]; }

void DeviceCollection::debugIn() {
  FILE *f = fopen("0.txt", "r");
  char buf[512];
  int i = 0;
  while (fscanf(f, "%s", buf) > 0) {
    buffer[i++] = unBinary(buf);
  }
  fclose(f);
}

void DeviceCollection::debugOut() {
  FILE *f;
  // if (id == 1)
  //   f = fopen("1.txt", "w");
  // else
  //   f = fopen("2.txt", "w");
  f = stdout;
  for (int i = 0; i < count; i++) {
    if (id == 1) {
      fprintf(f, " | ");
      for (int j = 0; j < 8; j++) {
        fprintf(f, "%s ", binary(buffer[i * 8 + j]));
      }
    } else {
      fprintf(f, "%s ", binary(buffer[i]));
    }
  }

  // fclose(f);
}

// void DeviceCollection::out () {
//     if (memcmp(buffer, oldBuffer, count*itemSize) != 0) {
//         #ifdef emu
//             printf ("%p ", buffer);
//         #endif
//         bcm2835_gpio_write(pin, LOW);
//         bcm2835_aux_spi_transfernb ((char *) buffer, NULL, count * itemSize);
//         bcm2835_gpio_write(pin, HIGH);
//         memcpy(oldBuffer, buffer, count*itemSize);
//         #ifdef emu
//             printf ("\n");
//         #endif
//     }
// }

// void DeviceCollection::in () {
//     bcm2835_aux_spi_transfernb (NULL, (char *) buffer, count * itemSize);
// }

void ShiftIn::init() {
  inputPin(dataPin);
  pinClr(dataPin);
  outputPin(inhPin);
  outputPin(shldPin);
  pinSet(inhPin);
  pinSet(shldPin);
}

void ShiftIn::in() {
  int i, j;
  unsigned char b;
#ifdef emu
  debugIn();
  return;
#endif
  // pinClr(shldPin);
  // waitMicroSec(1);
  // pinSet(shldPin);
  // waitMicroSec(1);
  // pinClr(inhPin);
  // waitMicroSec(1);
  // for (i = 0; i < count; i++) {
  //   b = 0;
  //   for (j = 7; j >= 0; j--) {
  //     b |= (pinGet(dataPin) << j);
  //     pinSet(sim.clockPin);
  //     waitMicroSec(1);
  //     pinClr(sim.clockPin);
  //     waitMicroSec(1);
  //   }
  //   buffer[i] = b;
  // }
  // pinSet(inhPin);
  // waitMicroSec(1);

  pinClr(shldPin);
  waitMicroSec(5);
  pinSet(shldPin);
  waitMicroSec(1);
  pinClr(inhPin);
  waitMicroSec(1);

  for (i = 0; i < count; i++) {
    b = 0;
    for (j = 7; j >= 0; j--) {
      b |= (pinGet(dataPin) << j);
      // waitMicroSec(1);
      pinSet(sim.clockPin);
      waitMicroSec(1);
      pinClr(sim.clockPin);
      waitMicroSec(1);
    }
    buffer[i] = b;
  }
  pinSet(inhPin);
  waitMicroSec(1);
}

void ShiftOut::init() {
  outputPin(dataPin);
  outputPin(latchPin);
  pinSet(latchPin);
  pinClr(dataPin);
}

void ShiftOut::out() {
  int i, j;
#ifdef emu
  debugOut();
  return;
#endif
  unsigned char b;

  // for (i = 0; i < count; i++) {
  //   b = buffer[i];
  //   for (j = 7; j >= 0; j--) {
  //     pinClr(sim.clockPin);
  //     waitMicroSec(1);
  //     pinOut(dataPin, (b >> j) & 1);
  //     pinSet(sim.clockPin);
  //     waitMicroSec(1);
  //   }
  // }
  // pinClr(sim.clockPin);
  // pinSet(latchPin);
  // waitMicroSec(1);
  // pinClr(latchPin);

  pinClr(latchPin);
  for (i = 0; i < count; i++) {
    b = buffer[i];
    for (j = 7; j >= 0; j--) {
      pinOut(dataPin, (b >> j) & 1);
      pinSet(sim.clockPin);
      waitMicroSec(1);
      pinClr(sim.clockPin);
    }
  }
  pinSet(latchPin);
}

void Shift7219::init() {
  outputPin(dataPin);
  outputPin(loadPin);
  pinClr(loadPin);
}

void Shift7219::byteOut(unsigned char b) {
  for (int j = 7; j >= 0; j--) {
    pinOut(dataPin, (b >> j) & 1);
    waitMicroSec(1);
    pinSet(sim.clockPin);
    waitMicroSec(1);
    pinClr(sim.clockPin);
    waitMicroSec(1);
  }
}

void Shift7219::command(unsigned char cmd, unsigned char v) {
  pinClr(loadPin);
  waitMicroSec(1);
  for (int i = 0; i < count; i++) {

    byteOut(cmd);
    byteOut(v);
    // for (int j = 0; j < i; j++) {
    //   byteOut(0);
    //   byteOut(0);
    // }
  }
  pinSet(loadPin);
  waitMicroSec(1);
}

void Shift7219::out() {
  for (int j = 0; j < 8; j++) {
    // command (j+1, buffer[i * 8 + j]);
    pinClr(loadPin);
    waitMicroSec(1);
    for (int i = 0; i < count; i++) {
      // printf("%d %s\n", j + 1, binary(buffer[i * 8 + j]));
      byteOut(j + 1);
      byteOut(buffer[i * 8 + j]);
    }
    pinSet(loadPin);
    waitMicroSec(1);
  }
  // pinSet (loadPin);
  // endOut();
}

unsigned char reverse(unsigned char c) {
  unsigned char r = 0;
  for (int i = 0; i < 8; i++) {
    r |= ((c >> i) & 1) << (7 - i);
  }
  return r;
}

char *binary(unsigned char c) {
  static char b[8 + 1];
  for (int i = 7; i >= 0; i--) {
    b[7 - i] = 48 + ((c >> i) & 1);
  }
  b[8] = 0;
  return b;
}

unsigned char unBinary(char *s) {
  int r = 0, d;
  for (int i = 0; i < 8; i++) {
    d = s[i] - 48;
    r += d << 7 - i;
  }
  return r;
}