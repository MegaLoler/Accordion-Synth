#pragma once

class Osc {
    private:
        double rate;
        double x = 0;

    public:
        int id;
        double freq = 0;
        double amp = 0;
        double pan = 0;

        Osc (int id, double rate) : id (id), rate (rate) {};
        double run ();
};
