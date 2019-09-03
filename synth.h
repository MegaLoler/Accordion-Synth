#pragma once

#include "osc.h"

class Synth {
    private:
        double rate = 0;
        double pressure = 0;
        double target_pressure = 0;
        double pressure_smoothing = 10;
        double target_pressures[128];
        double weights[128];
        double weight_smoothing = 0.99;

        // delay line reverb
        // TODO: make this rate independant
        const static int num_delays = 750;
        const static int room_center = 80;
        const double reflection = 0.8;
        const double reflection_stiffness = 0.1;
        double *delay_l;
        double *delay_r;
        double *delay_l_;
        double *delay_r_;
        double wet = 0.70;
        double dry = 0.30;
        //double wet = 0.00;
        //double dry = 1.00;

        double octave_multiplier_1 = 1;
        double octave_multiplier_2 = 0.5;
        double octave_multiplier_3 = 1;

        // pressure range
        //double max = 1.09;
        //double min = 0.88;
        double max = 1;
        double min = 0;//0.5;

        double amp = 1;
        Osc oscs[128];    // currently playing oscillators
        Osc oscs2[128];   // second set
        Osc oscs3[128];   // third set
        double detune = 0.025;    // semitones
        double pan_sep = 0.025;

        // low pass filter
        double beta = 0;//2;
        double low_pass_left = 0;
        double low_pass_right = 0;

        // control rate timer
        int control_rate_division = 1000;
        int clock = 0;

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
