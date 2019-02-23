#ifndef AUDIO_PROCESS_HPP
#define AUDIO_PROCESS_HPP

#include <QObject>

#include "AudioEngine.hpp"
#include "Synth.hpp"

class AudioProcess : public QObject, public AudioEngine
{
    Q_OBJECT

public:
    AudioProcess();

    ~AudioProcess();

    Synth<float> *synth;

private:

protected:
    virtual int processing_callback(const void *input_buffer,
                                    void *output_buffer,
                                    unsigned long frames_per_buffer,
                                    const PaStreamCallbackTimeInfo* time_info,
                                    PaStreamCallbackFlags status_flags);

signals:

};







#endif
