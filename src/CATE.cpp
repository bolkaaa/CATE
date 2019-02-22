/*
  CATE: Concatenative Audio Processing Application
  Copyright (c) 2019 Liam Wyllie. All rights reserved.

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <boost/filesystem.hpp>

#include "portaudiocpp/PortAudioCpp.hxx"
#include "portaudio.h"
#include "sndfile.hh"
#include "samplerate.h"

#include "Synth.hpp"
#include "AudioParameters.hpp"
#include "AudioBuffer.hpp"
#include "FileTree.hpp"
#include "Database.hpp"
#include "RingBuffer.hpp"
#include "FFT.hpp"
#include "Window.hpp"

using std::string;

int main(int argc, char *argv[])
{
    bool test = 0;
    bool init_db = 0;

    if (test)
    {

        return 0;
    }

    /* Initialise the PortAudio system and define audio parameters. */
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system, 48000, 256, 2, 2);

    /* Load database of buffers from root directory path. */
    if (init_db)
    {
        string audio_file_dir = "./audio_files";
        Database<float> db;
        db.add_directory(audio_file_dir);
        db.convert_sample_rates(audio_parameters.sample_rate());
    }

    uint16_t buffer_size = 1024;
    Synth<double> synth(buffer_size, audio_parameters.out_channels());

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<double> > stream(audio_parameters.stream(),
                                                          synth,
                                                          &Synth<double>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<double> >::start, &stream);

    /* Main thread */
    while (true)
    {
        /* Temporary until UI stuff implemented. */
    }

    audio_thread.join();
    std::cout << "Stopping PortAudio...\n";
    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
