#include <vector>
#include <unistd.h>
#include <stdint.h>
#include <RtMidi.h>
#include <rtaudio/RtAudio.h>
#include "synth.h"

Synth *synth;
int midi_channel = -1;      // -1 = omni
unsigned int rate = 44100;  // sampling rate
unsigned int frames = 32;   // audio buffer

void exit_error (std::string message) {
    std::cerr << message << std::endl;
    exit (EXIT_FAILURE);
}

void process_midi (double dt, std::vector<unsigned char> *message, void *user_data) {
    // convert this vector to an array
    int num_bytes = message->size ();
    uint8_t *data = new uint8_t[num_bytes];
    copy (message->begin (), message->end (), data);

    // midi channel masking
    if (midi_channel != -1) {
        uint8_t channel = data[0] & 0x0f;
        if (channel != midi_channel) {
            delete data;
            return;
        }
    }

    // send to the synth
    synth->midi (data);

    // done with the data
    delete data;
}

void setup_midi () {
    // setup midi input
    RtMidiIn *midi_in = new RtMidiIn ();
    std::vector<unsigned char> message;

    // check midi in ports
    unsigned int num_ports = midi_in->getPortCount ();
    if (num_ports == 0)
        exit_error ("Could not find MIDI input ports.");

    // open the first available port
    midi_in->openPort (0);

    // don't ignore messages
    midi_in->ignoreTypes (false, false, false);

    // set midi callback
    midi_in->setCallback (&process_midi);
}

int process_audio (void *output, void *input, unsigned int num_frames, double stream_time, RtAudioStreamStatus status, void *data) {
    double *buffer = (double *) output;

    if (status)
        std::cerr << "Underflow O_O!" << std::endl;

    // audio frames
    for (unsigned int i = 0; i < num_frames; i++)
        synth->run (&buffer[i * 2]);

    return 0;
}

int main (int argc, char **argv) {

    synth = new Synth ();
    synth->set_rate (rate);
    setup_midi ();

    RtAudio dac;
    if (dac.getDeviceCount () < 1) {
        exit_error ("No audio deviced found!");
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = dac.getDefaultOutputDevice ();
    parameters.nChannels = 2;
    parameters.firstChannel = 0;

    try {
        dac.openStream (&parameters, nullptr, RTAUDIO_FLOAT64, rate, &frames, &process_audio, nullptr);
        dac.startStream ();
    } catch (RtAudioError& e) {
        e.printMessage ();
        exit (EXIT_FAILURE);
    }

    while (true) {
        sleep (1);
    }

    try {
        dac.stopStream ();
    } catch (RtAudioError& e) {
        e.printMessage ();
    }

    if (dac.isStreamOpen ())
        dac.closeStream ();

    delete synth;
    return 0;
}
