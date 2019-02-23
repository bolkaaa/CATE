#include "AudioProcess.hpp"
#include "Synth.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>

AudioProcess::AudioProcess()
{
    synth = new Synth();
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
    static_cast<void>(status_flags);
    static_cast<void>(time_info);
    float *input = const_cast<float*>(static_cast<const float*>(input_buffer));
    float *output = static_cast<float*>(output_buffer);
    auto i = 0;

    for (i = 0; i < frames_per_buffer; ++i)
    {
        output[i] = input[i];
    }

    return paContinue;
}
