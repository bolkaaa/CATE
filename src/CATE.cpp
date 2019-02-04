#include <iostream>
#include <thread>
#include <vector>
#include <boost/filesystem.hpp>

#include "../lib/portaudio.h"
#include "../lib/PortAudioCpp.hxx"
#include "../lib/sndfile.h"
#include "../lib/sndfile.hh"

#include "Synth.hpp"
#include "AudioParameters.hpp"
#include "AudioBuffer.hpp"
#include "FileTree.hpp"

template <class T>
std::vector<AudioBuffer<T>> create_buffers(const std::string &audio_dir)
/* Example of creating container of AudioBuffers from a nested directory. */
{
    std::vector<std::string> file_list;
    get_nested_files(file_list, audio_dir);
    std::vector<AudioBuffer<T>> buffers;

    for (auto audio_file : file_list)
    {
	buffers.push_back(AudioBuffer<T>(audio_file));
    }

    return buffers;
}

int main(int argc, char *argv[])
{
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system, 20000, 256, 2, 2);

    /* Create std::vector of AudioBuffers and test Synth with one of the buffers. */
    std::string audio_file_dir = "./audio_files";
    std::vector<AudioBuffer<float>> buffers = create_buffers<float>(audio_file_dir);
    unsigned int test_buffer_index = 0;
    Synth<float> synth(buffers[test_buffer_index]);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<float>> stream(audio_parameters.get_stream(),
							 synth,
							 &Synth<float>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<float>>::start, &stream);
    audio_thread.detach();

    std::cout << "Press Ctrl-C to exit.\n";
    while(true)
    {
	std::cout << "OUTPUT: " << synth[synth.get_index()] << '\r';
    } // ...

    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
