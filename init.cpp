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

    while (fgets(buf, 80, f)) {
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
        }

    }
    for (int i = 0; i < sim.registers7219.length; i++) {
        Max7219 *m = (Max7219 *) sim.registers7219[i];
        printf ("%d %s %d\n", i, m->name.c_str(), m->commonAnode);
    }
    
    for (int i = 0; i < sim.sevenSegments.length; i++) {
        Sevenseg *m = (Sevenseg *) sim.sevenSegments[i];
        printf ("%d %s %s %d %d %s\n", i, m->name.c_str(), m->max7219->name.c_str(), m->digits, m->start, m->format.c_str());
    }
 }