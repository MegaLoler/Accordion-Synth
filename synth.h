#pragma once

#include <vector>
#include "osc.h"

class Synth {
    private:
        double rate = 0;
        double pressure = 0;
        std::vector<Osc> oscs;    // currently playing oscillators

    public:
        Synth ();
        ~Synth ();

        void set_rate (double rate);
        void note_on (int note, double velocity);
        void note_off (int note);
        void midi (uint8_t *data);
        void run (double *samples);
};
