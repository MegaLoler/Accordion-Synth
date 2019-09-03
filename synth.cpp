#include <iostream>
#include <cmath>
#include <stdint.h>
#include <algorithm>
#include "synth.h"

Synth::Synth () {
    for (int i = 0; i < 128; i++) {
        oscs[i].pan = (i / 127.0 - pan_sep) / 2 * M_PI;
        oscs[i].freq = 440 * pow (2.0, ((i - detune) - 69) / 12.0);
        oscs2[i].pan = (i / 127.0) / 2 * M_PI;
        oscs2[i].freq = 440 * pow (2.0, (i - 69) / 12.0) / 2;
        oscs3[i].pan = (i / 127.0 + pan_sep) / 2 * M_PI;
        oscs3[i].freq = 440 * pow (2.0, ((i + detune) - 69) / 12.0);
        target_pressures[i] = 0;
    }

    delay_r = new double[num_delays];
    delay_l = new double[num_delays];
    delay_r_ = new double[num_delays];
    delay_l_ = new double[num_delays];
}

Synth::~Synth () {
    delete delay_r;
    delete delay_l;
    delete delay_r_;
    delete delay_l_;
}

void Synth::set_rate (double rate) {
    this->rate = rate;
    for (int i = 0; i < 128; i++) {
        oscs[i].rate = rate;
        oscs2[i].rate = rate;
        oscs3[i].rate = rate;
    }
}

void Synth::set_pressure (double pressure) {
    this->pressure = pressure;

    for (int i = 0; i < 128; i++) {
        oscs[i].amp = pressure;
        oscs2[i].amp = pressure;
        oscs3[i].amp = pressure;
        weights[i] *= weight_smoothing;
    }
}

void Synth::note_on (int note, double velocity) {
    oscs[note].start ();
    oscs2[note].start ();
    oscs3[note].start ();
    target_pressures[note] = min + (max - min) * velocity;
    weights[note] = note * note;

    // calculate new target pressure
    double sum = 0;
    double total = 0;
    for (int i = 0; i < 128; i++) {
        double pressure = target_pressures[i] * weights[i];
        if (pressure) {
            sum += pressure;
            total += weights[i];
        }
    }
    target_pressure = sum / total;
}

void Synth::note_off (int note) {
    oscs[note].stop ();
    oscs2[note].stop ();
    oscs3[note].stop ();
    target_pressures[note] = 0;
    weights[note] = 0;
}

void Synth::run (double *samples) {
    set_pressure (pressure + (target_pressure - pressure) / (pressure_smoothing * control_rate_division));
    //if (clock++ % control_rate_division == 0) {
    //    set_pressure (pressure + (target_pressure - pressure) / pressure_smoothing);
    //}

    double l = 0;
    double r = 0;
    for (int i = 0; i < 128; i++) {
        if (oscs[i].is_on ()) {
            double sample = oscs[i].run ();
            l += cos (oscs[i].pan) * sample * amp;
            r += sin (oscs[i].pan) * sample * amp;
            sample = oscs2[i].run ();
            l += cos (oscs2[i].pan) * sample * amp;
            r += sin (oscs2[i].pan) * sample * amp;
            sample = oscs3[i].run ();
            l += cos (oscs3[i].pan) * sample * amp;
            r += sin (oscs3[i].pan) * sample * amp;
        }
    }

    // delay lines
    for (int i = 0; i < num_delays - 1; i++) {
        delay_l_[i] = delay_l[i + 1];
        delay_r_[i + 1] = delay_r[i];
    }
    delay_r_[0] = delay_l[0] * reflection;
    delay_l_[num_delays - 1] = delay_r[num_delays - 1] * reflection;

    delay_r_[0] = (delay_r_[0] * reflection_stiffness + delay_r[0]) / (1 + reflection_stiffness);
    delay_l_[num_delays - 1] = (delay_l_[num_delays - 1] * reflection_stiffness + delay_l[num_delays - 1]) / (1 + reflection_stiffness);


    double *tmp_r = delay_r;
    double *tmp_l = delay_l;
    delay_r = delay_r_;
    delay_l = delay_l_;
    delay_r_ = tmp_r;
    delay_l_ = tmp_l;
    delay_r[room_center] += r;
    delay_l[room_center] += l;

    // low pass filter
    low_pass_left  = (delay_r[room_center] * wet + l * dry + low_pass_left  * beta) / (1 + beta);
    low_pass_right = (delay_l[room_center] * wet + r * dry + low_pass_right * beta) / (1 + beta);
    samples[0] = low_pass_left;
    samples[1] = low_pass_right;
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
            target_pressure = min + (max - min) * (value / 127.0);
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
