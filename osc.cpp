#include "osc.h"

double Osc::run () {
    x += freq / rate;
    if (x > 1)
        x -= 2;
    return x * amp;
}
