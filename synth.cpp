#include <iostream>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include "synth.h"

Synth::Synth () {
    for (int i = 0; i < 128; i++) {
        oscs[i].pan = i / 127.0 / 2 * M_PI;
    }
}

Synth::~Synth () {

}

void Synth::set_rate (double rate) {
    this->rate = rate;
    for (int i = 0; i < 128; i++) {
        oscs[i].rate = rate;
    }
}

void Synth::set_pressure (double pressure) {
    this->pressure = pressure;
    for (int i = 0; i < 128; i++) {
        oscs[i].amp = pressure;
    }
}

void Synth::note_on (int note, double velocity) {
    Osc &osc = oscs[note];
    osc.on = true;
    osc.freq = 440 * pow (2.0, (note - 69) / 12.0);
}

void Synth::note_off (int note) {
    oscs[note].on = false;
}

void Synth::run (double *samples) {
    samples[0] = samples[1] = 0;
    double l = 0;
    double r = 0;
    for (int i = 0; i < 128; i++) {
        if (oscs[i].on) {
            l = r = oscs[i].run ();
            samples[0] += cos (oscs[i].pan) * l * amp;
            samples[1] += sin (oscs[i].pan) * r * amp;
        }
    }
}

void Synth::midi (uint8_t *data) {

    // parse the data
    uint8_t type = data[0] & 0xf0;
    uint8_t chan = data[0] & 0x0f;

    std::cout << "Received midi event: 0x" << std::hex << (int) type
        << " channel: 0x" << std::hex << (int) chan << std::endl;

    if (type == 0xb0) {

        // handle control events
        uint8_t id = data[1];
        uint8_t value = data[2];

        std::cout << "Received midi controller event: 0x" << std::hex << (int) id
            << " 0x" << std::hex << (int) value << std::endl;

        if (id == 0x15) {
            set_pressure (value / 127.0);
        }

    } else if (type == 0x80) {

        // handle note off events
        uint8_t note = data[1];

        std::cout << "Received midi note off event: 0x" << std::hex << (int) note << std::endl;

        note_off (note);

    } else if (type == 0x90) {

        // handle note on events
        uint8_t note = data[1];
        uint8_t velocity = data[2];

        std::cout << "Received midi note on event: 0x" << std::hex << (int) note
            << " 0x" << std::hex << (int) velocity << std::endl;

        note_on (note, velocity / 127.0);

    } else if (type == 0xe0) {

        // handle pitch bends
        uint8_t msb = data[2];

        std::cout << "Received pitch bend event: 0x" << std::hex << (int) msb << std::endl;

    } else if (type == 0xc0) {

        // handle program changes
        uint8_t id = data[1];

        std::cout << "Received program change event: 0x" << std::hex << (int) id << std::endl;

    }

}
