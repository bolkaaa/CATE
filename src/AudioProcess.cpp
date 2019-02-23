#include "AudioProcess.hpp"
#include "Synth.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

AudioProcess::AudioProcess()
{
    synth = new Synth<float>(1024, 2);
}

AudioProcess::~AudioProcess()
{
    delete synth;
}

int AudioProcess::processing_callback(const void *input_buffer,
                                      void *output_buffer,
                                      unsigned long frames_per_buffer,
                                      const PaStreamCallbackTimeInfo* time_info,
                                      PaStreamCallbackFlags status_flags)
{
    float *output = (float*) output_buffer;
    float *input = (float*) input_buffer;
    (void) status_flags;
    (void) time_info;
    auto i = 0;

    for (i = 0; i < frames_per_buffer; ++i)
    {
        output[i] = input[i];
    }

    return paContinue;
}
