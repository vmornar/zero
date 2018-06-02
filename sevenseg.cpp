#include <stdio.h>
#include <string.h>
#include <string>
using namespace std;
#include "max7219.h"
#include "sevenseg.h"

unsigned char Sevenseg::segments[] = {
    //.ABCDEFG  Segments
    0b00000000, // ' '
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0b00000001, // -
    0,          // .
    0,
    0b01111110, // 0
    0b00110000, // 1
    0b01101101, // 2
    0b01111001, // 3
    0b00110011, // 4
    0b01011011, // 5
    0b01011111, // 6
    0b01110000, // 7
    0b01111111, // 8
    0b01111011, // 9
    0, 0, 0, 0, 0, 0, 0,
    0b01110111, // A
    0b00011111, // b
    0b10011100, // C    0b00011010, //c
    0b01111010, // d
    0b01001111, // E
    0b01000111, // F
    0,
    0b00110111, // H    0b00101110, //h
    0b00110000, // I
    0b00111100, // J
    //.ABCDEFG  Segments
    0,
    0b00001110, // L
    0, 0,
    0b01111110, // O  0b00111010, //o
    0b01100111, // P
    0, 0,
    0b01011011, // S
    0b00001111, // t
    0b00111110, // U
    0b00111110, // V
    0,
    0b00110011, // Y
    0b01101101, // Z
    0b11111111  // Z
};

void Sevenseg::init() {
  display = std::string(digits, ' ');
  max7219->setContent(display.c_str(), start, digits);
}

void Sevenseg::setValue(int val) {
  char buf[8 + 1];
  value = val;
  sprintf(buf, format.c_str(), val);
  display = buf;
  max7219->setContent(buf, start, digits);
  // memcpy (max7219->content+start, buf, digits);
}

void Sevenseg::setStr(string s) {
  display = s;
  max7219->setContent(display.c_str(), start, digits);
}

int Sevenseg::getValue() { return value; }
