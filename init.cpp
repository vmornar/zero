#include "main.h"
//#include <stdarg.h>

#define MAXBUF 256
void log(string message) {
  FILE *f;
  f = fopen("log.txt", "a");
  fprintf(f, "%s\n", message.c_str());
  fclose(f);
}

void fatal(string message) {
  fprintf(stderr, "%s\n", message.c_str());
  log(message);
  exit(1);
}

// void F(char *format, ...) {
//   va_list arglist;
//   va_start(arglist, format);
// 	vfprintf(stderr, format, arglist);
// 	va_end(arglist);
//   exit(1);
// }

void convertBitNumber(int &bit) {
  if (bit >= 'A')
    bit -= 'A';
  else
    bit -= '0';
}

// int findIndex (char *name, char *vars[], int nVars) {
//   for (int i=0; i < nVars; i++) {
//     if (strcmp (name, vars[i]) == 0) return i;
//   }
//   std::string sname(name);
//    ("Cant'find variable" + sname);
// }

Device *findVar(string varName) {
  Device *p;
  try {
    p = sim.vars[varName]; // findIndex (varName, vars, nVars);
    if (p != NULL)
      return p;
    throw(new exception); //    ("Cant'find var " + std::string(varName));
  } catch (...) {
    ("Cant'find var " + std::string(varName));
  }
}

int findVar(std::unordered_map<std::string, int> vars, char *varName) {
  try {
    return vars.at(varName); // findIndex (varName, vars, nVars);
  } catch (...) {
    ("Cant'find var " + std::string(varName));
  }
}

void init() {
  char buf[MAXBUF + 1], name[MAXBUF + 1], parentName[MAXBUF + 1],
      format[MAXBUF + 1], varName[MAXBUF + 1];
  // char **vars = NULL;
  int nVars;
  int commonAnode, pin;
  std::unordered_map<std::string, int> vars;

  FILE *f;
  f = fopen("Vars.txt", "r");
  if (!f)
    ("Can't open Vars.txt");

  nVars = 0;
  while (fgets(buf, MAXBUF, f) != NULL) {
    if (buf[0] == '#')
      continue;
    sscanf(buf, "%s", name);
    
    vars.emplace(name, nVars);
    printf("%d %s %d\n", nVars, name, vars.at(name));
    nVars++;
  }
  sim.devices = (Device **)malloc(nVars * sizeof(Device *));
  for (int i = 0; i < nVars; i++) {
    sim.devices[i] = NULL;
  }

  fclose(f);

  f = fopen("config.txt", "r");
  if (!f)
    ("Can't open config.txt");

  sim.registers7219.itemSize = 8;

  // bcm2835_gpio_fsel(16, BCM2835_GPIO_FSEL_OUTP);
  // bcm2835_gpio_fsel(20, BCM2835_GPIO_FSEL_OUTP);
  // sim.shiftOuts.pin = 3;
  // bcm2835_gpio_fsel(21, BCM2835_GPIO_FSEL_OUTP);

  while (fgets(buf, 80, f)) {

    if (buf[0] == '#')
      continue;

    if (strncmp(buf, "SIMULATOR", 9) == 0) {
      sscanf(buf, "%*s %s %d", name, &sim.clockPin);
    } else if (strncmp(buf, "SHIFTIN", 7) == 0) {
      sscanf(buf, "%*s %s %d", name, &pin);
      if (strcmp(name, "DATAPIN") == 0) {
        sim.shiftIns.dataPin = pin;
      } else if (strcmp(name, "SHLDPIN") == 0) {
        sim.shiftIns.shldPin = pin;
      } else if (strcmp(name, "INHPIN") == 0) {
        sim.shiftIns.inhPin = pin;
      }
    } else if (strncmp(buf, "SHIFTOUT", 8) == 0) {
      sscanf(buf, "%*s %s %d", name, &pin);
      if (strcmp(name, "DATAPIN") == 0) {
        sim.shiftOuts.dataPin = pin;
      } else if (strcmp(name, "LATCHPIN") == 0) {
        sim.shiftOuts.latchPin = pin;
      }
    } else if (strncmp(buf, "SHIFT7219", 9) == 0) {
      sscanf(buf, "%*s %s %d", name, &pin);
      if (strcmp(name, "DATAPIN") == 0) {
        sim.registers7219.dataPin = pin;
      } else if (strcmp(name, "LOADPIN") == 0) {
        sim.registers7219.loadPin = pin;
      }
    } else if (strncmp(buf, "7219", 4) == 0) {
      Max7219 *m7219 = new Max7219();
      sim.registers7219.add(m7219);
      sscanf(buf, "%*s %s %d", name, &(m7219->commonAnode));
      m7219->name = name;
    } else if (strncmp(buf, "7SEG", 4) == 0) {
      Sevenseg *s7 = new Sevenseg();
      sim.sevenSegments.add(s7);
      sscanf(buf, "%*s %s %s %d %d %s %s", name, parentName, &(s7->digits),
             &(s7->start), format, varName);
      s7->name = name;
      s7->format = format;
      s7->simIndex = findVar(vars, varName);
      sim.devices[s7->simIndex] = s7;
      if (!(s7->max7219 = (Max7219 *)sim.registers7219.find(parentName)))
        ("Can't find parent for 7seg " + s7->name);
      sim.vars.emplace(s7->name, s7);
      s7->init();
    } else if (strncmp(buf, "SI", 2) == 0 || strncmp(buf, "SO", 2) == 0) {
      ShiftReg *sr = new ShiftReg();
      if (strncmp(buf, "SI", 2) == 0)
        sim.shiftIns.add(sr);
      else
        sim.shiftOuts.add(sr);
      sscanf(buf, "%*s %s", name);
      sr->name = name;
    } else if (strncmp(buf, "BI", 2) == 0 || strncmp(buf, "BO", 2) == 0) {
      Bit *bit = new Bit();

      if (strncmp(buf, "BI", 2) == 0)
        sim.bitIns.add(bit);
      else
        sim.bitOuts.add(bit);

      sscanf(buf, "%*s %s %s %c %s", name, parentName, &(bit->bit), varName);

      bit->simIndex = findVar(vars, varName);
      sim.devices[bit->simIndex] = bit;

      convertBitNumber(bit->bit);

      bit->name = name;
      sim.vars.emplace(bit->name, bit);
      if (!(bit->shiftReg = strncmp(buf, "BI", 2) == 0
                                ? (ShiftReg *)sim.shiftIns.find(parentName)
                                : (ShiftReg *)sim.shiftOuts.find(parentName)))
        ("Can't find parent for bi " + bit->name);
    } else if (strncmp(buf, "RE", 2) == 0) {
      RotaryEncoder *re = new RotaryEncoder();
      sim.rotaryEncoders.add(re);
      sscanf(buf, "%*s %s %s %c %c %s", name, parentName, &(re->bitA),
             &(re->bitB), varName);
      re->simIndex = findVar(vars, varName);
      convertBitNumber(re->bitA);
      convertBitNumber(re->bitB);
      re->name = name;
      sim.vars.emplace(re->name, re);
      if (!(re->shiftReg = (ShiftReg *)sim.shiftIns.find(parentName)))
        ("Can't find parent for bi " + re->name);
    }
  }

  fclose(f);

  printf("Clockpin: %d\n", sim.clockPin);
  printf("Shiftin datapin: %d\n", sim.shiftIns.dataPin);
  printf("Shiftin shldpin: %d\n", sim.shiftIns.shldPin);
  printf("Shiftin inhpin: %d\n", sim.shiftIns.inhPin);
  printf("Shiftout datapin: %d\n", sim.shiftOuts.dataPin);
  printf("Shiftout latchpin: %d\n", sim.shiftOuts.latchPin);
  printf("Shift7219 datapin: %d\n", sim.registers7219.dataPin);
  printf("Shift7219 loadpin: %d\n", sim.registers7219.loadPin);

  for (int i = 0; i < sim.registers7219.count; i++) {
    Max7219 *m = (Max7219 *)sim.registers7219[i];
    printf("7219 %d %s %d\n", i, m->name.c_str(), m->commonAnode);
  }

  for (int i = 0; i < sim.sevenSegments.count; i++) {
    Sevenseg *m = (Sevenseg *)sim.sevenSegments[i];
    printf("7SEG %d %s %s %d %d %s %d\n", i, m->name.c_str(),
           m->max7219->name.c_str(), m->digits, m->start, m->format.c_str(),
           m->simIndex);
  }

  for (int i = 0; i < sim.shiftIns.count; i++) {
    ShiftReg *m = (ShiftReg *)sim.shiftIns[i];
    printf("SI %d %s\n", i, m->name.c_str());
  }

  for (int i = 0; i < sim.bitIns.count; i++) {
    Bit *m = (Bit *)sim.bitIns[i];
    printf("BI %d %s %s %d\n", i, m->name.c_str(), m->shiftReg->name.c_str(),
           m->simIndex);
  }

  for (int i = 0; i < sim.shiftOuts.count; i++) {
    ShiftReg *m = (ShiftReg *)sim.shiftOuts[i];
    printf("SO %d %s\n", i, m->name.c_str());
  }

  for (int i = 0; i < sim.bitOuts.count; i++) {
    Bit *m = (Bit *)sim.bitOuts[i];
    printf("BO %d %s %s %d\n", i, m->name.c_str(), m->shiftReg->name.c_str(),
           m->simIndex);
  }

  sim.registers7219.init();

  sim.registers7219.command(MAX7219_INTENSITY_REG, 15);
  sim.registers7219.command(MAX7219_SCANLIMIT_REG, 7);
  sim.registers7219.command(MAX7219_DECODE_REG, 0);
  sim.registers7219.command(MAX7219_DISPLAYTEST_REG, 1);
  sim.registers7219.command(MAX7219_DISPLAYTEST_REG, 0);

  sim.registers7219.command(MAX7219_SHUTDOWN_REG, 1);

  sim.shiftIns.init();
  sim.shiftOuts.init();
}