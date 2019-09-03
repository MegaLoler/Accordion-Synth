#pragma once

class Osc {
    private:
        double x = 0;

        double rest = 0.1;
        double width = 1;
        double thickness = 0.05;
        double clearance = 0.05;

        double last = 0;
        double sample_last = 0;
        double lf = 0;

        double alpha = 0.9;     // high pass
        double beta = 2;        // low pass

    public:
        double rate = 0;
        double freq = 0;
        double amp = 0;
        double pan = 0;
        bool on = false;

        double run ();
};
