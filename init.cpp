#include "main.h"
 
 void fatal (string message) {
     fprintf (stderr, message.c_str());
     exit(1);
 }

 void init () {
    char buf[80+1], name[80+1], parentName[80+1], format[80+1];
    int commonAnode;
    FILE *f = fopen ("config.txt", "r");
    if (!f) fatal ("Can't open config.txt");

    fprintf (stderr, "I0\n");
    sim.registers7219.pin = 1; sim.registers7219.itemSize = 16;
    fprintf (stderr, "I0a\n");

    bcm2835_gpio_fsel(16, BCM2835_GPIO_FSEL_OUTP);

    fprintf (stderr, "I0b\n");
    
    sim.sevenSegments.pin = -1;
    sim.shiftIns.pin = 2;

    fprintf (stderr, "I1\n");

    bcm2835_gpio_fsel(20, BCM2835_GPIO_FSEL_OUTP);
    sim.shiftOuts.pin = 3;
    bcm2835_gpio_fsel(21, BCM2835_GPIO_FSEL_OUTP);

    fprintf (stderr, "I2\n");
    while (fgets(buf, 80, f)) {
        if (buf[0] == '#') continue;
        if (strncmp (buf, "7219", 4) == 0) {
            Max7219 *m7219 = new Max7219();
            sim.registers7219.add(m7219);
            sscanf (buf, "%*s %s %d", name, &(m7219->commonAnode));
            m7219->name = name;
        } else if (strncmp (buf, "7SEG", 4) == 0) {
            Sevenseg *s7 = new Sevenseg();
            sim.sevenSegments.add (s7);
            sscanf (buf, "%*s %s %s %d %d %s", name, parentName, &(s7->digits), &(s7->start), format);
            s7->name = name;
            s7->format = format;
            if (!(s7->max7219 = (Max7219 *) sim.registers7219.find(parentName))) fatal ("Can't find parent for 7seg " + s7->name);
        } else if (strncmp (buf, "SI", 2) == 0 || strncmp (buf, "SO", 2) == 0) {
            ShiftReg *sr = new ShiftReg();
            if (strncmp (buf, "SI", 2) == 0) sim.shiftIns.add (sr);
            else sim.shiftOuts.add (sr);
            sscanf (buf, "%*s %s", name);
            sr->name = name;
        } else if (strncmp (buf, "BI", 2) == 0 || strncmp (buf, "BO", 2) == 0) {
            Bit *bit = new Bit();
            if (strncmp (buf, "BI", 2) == 0) sim.bitIns.add(bit);
            else sim.bitOuts.add(bit);
            sscanf (buf, "%*s %s %s %d", name, parentName, &(bit->bit));
            bit->name = name;
            if (!(bit->shiftReg = strncmp (buf, "BI", 2) == 0 ? (ShiftReg *) sim.shiftIns.find(parentName) : (ShiftReg *) sim.shiftOuts.find(parentName)))
                fatal ("Can't find parent for bi " + bit->name);
        }
    }

    for (int i = 0; i < sim.registers7219.count; i++) {
        Max7219 *m = (Max7219 *) sim.registers7219[i];
        printf ("7219 %d %s %d\n", 
            i, m->name.c_str(), m->commonAnode);
    }
    
    for (int i = 0; i < sim.sevenSegments.count; i++) {
        Sevenseg *m = (Sevenseg *) sim.sevenSegments[i];
        printf ("7SEG %d %s %s %d %d %s\n", 
            i, m->name.c_str(), m->max7219->name.c_str(), m->digits, m->start, m->format.c_str());
    }

    for (int i = 0; i < sim.shiftIns.count; i++) {
        ShiftReg *m = (ShiftReg *) sim.shiftIns[i];
        printf ("SI %d %s\n", i, m->name.c_str());
    }

    for (int i = 0; i < sim.bitIns.count; i++) {
        Bit *m = (Bit *) sim.bitIns[i];
        printf ("BI %d %s %s\n", i, m->name.c_str(), m->shiftReg->name.c_str());
    }

    for (int i = 0; i < sim.shiftOuts.count; i++) {
        ShiftReg *m = (ShiftReg *) sim.shiftOuts[i];
        printf ("SO %d %s\n", i, m->name.c_str());
    }

    for (int i = 0; i < sim.bitOuts.count; i++) {
        Bit *m = (Bit *) sim.bitOuts[i];
        printf ("BO %d %s %s\n", i, m->name.c_str(), m->shiftReg->name.c_str());
    }
 }