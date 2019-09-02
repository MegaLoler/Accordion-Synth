class Synth {
    private:
        double rate;
        double osc;
        double freq;

    public:
        Synth ();
        ~Synth ();

        void set_rate (double rate);
        void note_on (int note, double velocity);
        void note_off (int note);
        void midi (uint8_t *data);
        void run (double *samples);
};
