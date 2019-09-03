#include <cmath>
#include "osc.h"

double Osc::run () {
    x += freq / rate;
    double osc = sin (x * 2 * M_PI) * amp * 0.85;
    
    double position = rest + osc;
    double x_up = position + thickness / 2.0;
    
    double side = 0;
    // crude integration
    const int res = 100;
    for (int i = 0; i < res; i++) {
        double j = i / res;
        double m = j * j * (6 - 4 * j + j * j) / 24.0;
        side += sqrt (pow (x_up * m, 2.0) + clearance * clearance) / res;
    }
    double tmp = clearance - osc;
    double end = (width + clearance) * sqrt (abs (x_up) + pow(tmp, 2.0));
    double tri = abs (x_up) * tmp;
    double section = end + tri + 2 * side;

    double sample = section;
    last = alpha * (last + sample - sample_last);
    sample_last = sample;
    lf = (last + lf * beta) / (1 + beta);
    return lf;
}
