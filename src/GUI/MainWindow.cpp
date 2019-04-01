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

    /* Buttons */
    connect(ui->start_playback, SIGNAL(pressed()), this, SLOT(start_playback_button_pressed()));
    connect(ui->stop_playback, SIGNAL(pressed()), this, SLOT(stop_playback_button_pressed()));
    connect(ui->start_recording, SIGNAL(pressed()), this, SLOT(start_recording_button_pressed()));
    connect(ui->stop_recording, SIGNAL(pressed()), this, SLOT(stop_recording_button_pressed()));

    /* Sliders */
    connect(ui->amplitude_slider, SIGNAL(valueChanged(int)), this, SLOT(set_amplitude(int)));
    connect(ui->grain_attack_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_attack(int)));
    connect(ui->grain_release_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_release(int)));
    connect(ui->grain_density_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_density(int)));
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

void MainWindow::set_amplitude(int new_value)
{
    float amplitude = new_value / 127.0f;
    audio_process.set_amplitude(amplitude);
}

void MainWindow::set_grain_attack(int new_value)
{
    float attack = new_value / 127.0f;
    audio_process.set_grain_attack(attack);
}

void MainWindow::set_grain_release(int new_value)
{
    float decay = new_value / 127.0f;
    audio_process.set_grain_release(decay);
}


void MainWindow::set_grain_density(int new_value)
{
    audio_process.set_grain_density(new_value);
}

} // CATE
