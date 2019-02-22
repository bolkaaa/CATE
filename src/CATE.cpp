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
    /* Initialise GUI settings. */
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);
    QIcon appIcon;
    appIcon.addFile(":/Icons/AppIcon32");
    appIcon.addFile(":/Icons/AppIcon128");
    app.setWindowIcon(appIcon);
    MainWindow main_window;

    main_window.show();

    return app.exec();
}
