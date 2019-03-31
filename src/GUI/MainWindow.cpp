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

#include <QDebug>
#include <portaudio.h>

#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioProcess.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

namespace CATE {

MainWindow::MainWindow(QWidget *parent, AudioProcess &audio_process)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          audio_process(audio_process)
{
    ui->setupUi(this);

    /* Connect Signals. */
    connect(ui->start_playback_button_pressed, SIGNAL(pressed()), this, SLOT(start_playback_button_pressed()));
    connect(ui->stop_playback_button_pressed, SIGNAL(pressed()), this, SLOT(stop_playback_button_pressed()));
    connect(ui->start_recording_button_pressed, SIGNAL(pressed()), this, SLOT(start_recording_button_pressed()));
    connect(ui->stop_recording_button_pressed, SIGNAL(pressed()), this, SLOT(stop_recording_button_pressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_playback_button_pressed()
{
    audio_process.start_stream();
}

void MainWindow::stop_playback_button_pressed()
{
    audio_process.stop_stream();
}

void MainWindow::start_recording_button_pressed()
{
    audio_process.start_recording();
}

void MainWindow::stop_recording_button_pressed()
{
    audio_process.stop_recording();
}


} // CATE
