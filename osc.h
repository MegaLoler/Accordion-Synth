#pragma once

class Osc {
    private:
        double x = 0;

        //double volume = 0.5;
        //double rest = 0.528;
        //double length = 12.95;
        //double width = 2.1;
        //double thickness = 0.11;
        //double clearance = 0.05;

        double volume = 1;
        //double rest = 0.3;
        double rest = 0.1;
        double length = 1;
        double width = 1;
        double thickness = 0.05;
        double clearance = 0.05;

        //double max = 0.85;
        //double max = 0.88;
        double max = 1.09;
        double min = 0.88;
        //double min = 0;
        //double max = 1;
        //double max = 1.3;

        // high pass filter
        double alpha = 0.9;     // high pass

        double last = 0;
        double high_pass = 0;

        double env = 0;
        double env_smooth = 500; // bad filter lol
        double epsilon = 0.0001;

        bool running = false;

    public:
        double rate = 0;
        double freq = 0;
        double amp = 0;
        double pan = 0;
        bool on = false;

        double run ();
        void start ();
        void stop ();
        bool is_on ();
};
