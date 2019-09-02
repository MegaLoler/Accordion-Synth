class Synth {
    private:
        double rate;
        double osc;
        double freq;

    public:
        Synth ();
        ~Synth ();

        void set_rate (double rate);
        void midi (uint8_t *data);
        void run (double *samples);
};
