#include <string>
#include <stdio.h>
using namespace std;

#include "sevenseg.h"

class sevenseg {
private:
        int reg;
        int digits;
        int start;
        string content;
public:
    sevenseg (int reg, int digits, int start) :
        reg(reg), digits(digits), start(start), content("")
    {
    }

    void setInt (int val) {
        char buf[digits+1];
        sprintf (buf, ("""%"+to_string(digits)+"d""").c_str(), val);
        content = buf;
    }
    void setStr (char *s) {
        content = s;
    }
};