#include <string>
#include <stdio.h>
using namespace std;
#include "max7219.h"
#include "sevenseg.h"

unsigned char segments[] = {
    // GFEDCBA  Segments
    0b00000000, // = ' '
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0b01000000,
    0, 0, 
    0b01111110, // = 0
    0b00110000, // = 1
    0b01101101, // = 2
    0b01111001, // = 3
    0b00110011, // = 4
    0b01011011, // = 5
    0b01011111, // = 6
    0b01110000, // = 7
    0b01111111, // = 8
    0b01111011, // = 9
    0, 0, 0, 0, 0, 0, 0,
    0B11101110, //A
    0B00111110, //B
    0B10011100, //C
    0B00011010, //c
    0B01111010, //d
    0B10011110, //E
    0B10001110, //F
    0B01101110, //H
    0B00101110, //h
    0B00011100, //L
    0B01100000, //l
    0B11111100, //O
    0B00111010, //o
    0B11001110, //P
    0B10110110, //S
    0B11111111
};


void Sevenseg::setInt (int val) {
    char buf[8+1];
    sprintf (buf, format.c_str(), val);
    content = buf;
    memcpy (max7219->content+start, buf, digits);
}

void Sevenseg::setStr (char *s) {
    content = s;
}

unsigned char Sevenseg::reverse (unsigned char c) {
    unsigned char r = 0;
    for (int i = 0; i < 8; i++) {
        r |= ((c >> i) & 1) << (7-i);
    }
}

char * Sevenseg::binary (unsigned char c) {
    static char b[8+1];
    for (int i = 7; i >= 0; i--) {
        b[7-i] = 48 + ((c >> i) & 1);
    }
    b[8] = 0;
    return b;
}
