#ifndef AUDIO_ENGINE_HPP
#define AUDIO_ENGINE_HPP

#include <portaudio.h>

class AudioEngine
{
public:
    AudioEngine();

    ~AudioEngine();

    void init();

    int start_stream();

    int stop_stream();

private:

protected:
    /* The main processing callback function. */
    virtual int processing_callback(const void *input_buffer,
                                    void *output_buffer,
                                    unsigned long frames_per_buffer,
                                    const PaStreamCallbackTimeInfo* time_info,
                                    PaStreamCallbackFlags status_flags);

    /* With PortAudio, a static callback function must be defined, which
       returns the custom callback function. */
    static int static_callback(const void *input_buffer,
                               void *output_buffer,
                               unsigned long frames_per_buffer,
                               const PaStreamCallbackTimeInfo* time_info,
                               PaStreamCallbackFlags status_flags,
                               void *user_data)
    {
        return ((AudioEngine*) user_data)->processing_callback(input_buffer,
                                                               output_buffer,
                                                               frames_per_buffer,
                                                               time_info,
                                                               status_flags);
    }

    PaError error;
    PaStream *stream;
    PaTime suggested_latency;
    PaHostApiIndex selected_host_api;
    PaStreamParameters input_params;
    PaStreamParameters output_params;
    double sample_rate;
    int frames_per_buffer;
    bool is_running;
};

#endif
