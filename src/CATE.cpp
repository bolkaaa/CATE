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

int main(int argc, char *argv[])
{
    bool test = false;
    bool init_db = false;

    if (test)
    {
        // ...
        return 0;
    }

    /* Initialise the PortAudio system and define audio parameters. */
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system, 48000, 256, 2, 2);

    /* Load database of buffers from root directory path. */
    if (init_db)
    {
        std::string audio_file_dir = "./audio_files";
        Database<float> db;
        db.add_directory(audio_file_dir);
        db.convert_sample_rates(audio_parameters.sample_rate());
    }

    Synth<float> synth(1024);

    /* Create a PortAudio stream for the Synth instance and start it on a new thread. */
    portaudio::MemFunCallbackStream<Synth<float> > stream(audio_parameters.stream(),
                                                          synth,
                                                          &Synth<float>::process);
    std::thread audio_thread(&portaudio::MemFunCallbackStream<Synth<float> >::start, &stream);
    audio_thread.detach();

    /* Main thread */
    std::cout << "Enter any non-zero character to exit.\n";
    int terminate = 0;
    while (std::cin >> terminate)
    {
        if (terminate != 0)
        {
            break;
        }
    }

    std::cout << "Stopping PortAudio...\n";
    stream.stop();
    stream.close();
    system.terminate();

    return 0;
}
