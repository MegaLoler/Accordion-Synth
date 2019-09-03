#include <cmath>
#include "osc.h"

double Osc::run () {
    double t_env = on ? 1 : 0;
    env += (t_env - env) / env_smooth;
    if (!on && env < epsilon)
        running = false;

    double amp_ = (min + amp * (max - min)) * env;

    double osc = sin (x * 2 * M_PI) * amp_;
    x += freq / rate;
    
    double position = rest + osc;
    double x_up = position + thickness / 2.0;
    
    double side = 0;
    // crude integration
    const int res = 10;
    for (int i = 0; i < res; i++) {
        double j = i / res;
        double m = j * j * (6 - 4 * j + j * j) / 24.0;
        side += sqrt (pow (x_up * m, 2.0) + clearance * clearance) / res;
    }
    double tmp = clearance - osc;
    double end = (width + clearance) * sqrt (abs (x_up) + pow(tmp, 2.0));
    double tri = abs (x_up) * tmp;
    double section = end + tri + 2 * side * length;

    // high pass filter
    double sample = section * amp_;
    high_pass = alpha * (high_pass + sample - last);
    last = sample;
    return high_pass * volume;
}

bool Osc::is_on () {
    return running;
}

void Osc::start () {
    on = true;
    running = true;
}

void Osc::stop () {
    on = false;
}
