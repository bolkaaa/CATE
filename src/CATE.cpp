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
#include <QApplication>
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
#include "MainWindow.hpp"

using std::string;

int main(int argc, char *argv[])
{
    bool test = 0;
    bool init_db = 0;

    if (test)
    {

    }

    /* Initialise GUI settings. */
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QIcon appIcon;
    appIcon.addFile(":/Icons/AppIcon32");
    appIcon.addFile(":/Icons/AppIcon128");
    app.setWindowIcon(appIcon);
    MainWindow main_window;

    /* Initialise the PortAudio system and define audio parameters. */
    portaudio::AutoSystem auto_system;
    portaudio::System &system = portaudio::System::instance();
    AudioParameters audio_parameters(system, 48000, 1024, 2, 2);

    Synth<double> synth(audio_parameters.frames_per_buffer(),
                        audio_parameters.out_channels());

    portaudio::MemFunCallbackStream<Synth<double> > stream(audio_parameters.stream(),
                                                           synth,
                                                           &Synth<double>::process);

    stream.start();

    system.sleep(2000);

    std::cout << "Stopping PortAudio...\n";
    stream.stop();
    stream.close();
    system.terminate();

    // main_window.show();

    return app.exec();
}
