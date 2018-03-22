#ifndef __DEVICE
#define __DEVICE
#include <stdio.h>
#include <string>
using namespace std;

class Device {
public:
    string name;
};

class DeviceCollection {
private:
    Device **p = NULL;
public:
    int length = 0;
    Device * find (string name) {
        for (int i=0; i < length; i++) {
            if (name == p[i]->name) return p[i];
        }
        return NULL;
    }
    void add (Device *d) {
        p = (Device **) realloc(p, sizeof (Device *) * (++length));
        p[length-1] = d;
    }
    Device * operator[] (int i) {
        return p[i]; 
    }
};
#endif