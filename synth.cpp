#include <stdint.h>
#include "synth.h"

Synth::Synth () {
    osc = 0;
    freq = 440;
}

Synth::~Synth () {

}

void Synth::set_rate (double rate) {
    this->rate = rate;
}

void Synth::midi (uint8_t *data) {

}

void Synth::run (double *samples) {
    osc += freq / rate;
    if (osc > 1)
        osc -= 2;
    double sample = osc;
    samples[0] = samples[1] = sample;
}
