#pragma once

#include "osc.h"

class Synth {
    private:
        double rate = 0;
        double pressure = 0;
        double amp = 0.5;
        Osc oscs[128];    // currently playing oscillators

    public:
        Synth ();
        ~Synth ();

        void set_rate (double rate);
        void set_pressure (double pressure);
        void note_on (int note, double velocity);
        void note_off (int note);
        void midi (uint8_t *data);
        void run (double *samples);
};
