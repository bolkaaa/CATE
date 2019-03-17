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

MainWindow::MainWindow(QWidget *parent, Database &db, PointCloud &point_cloud, KdTree &kd_tree)
        : QMainWindow(parent),
          ui(new Ui::MainWindow),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree),
          sample_rate(48000),
          frames_per_buffer(256),
          fft_bin_size(1024)
{
    ui->setupUi(this);

    audio_process = new AudioProcess(sample_rate, frames_per_buffer, 1, 1, fft_bin_size, db, point_cloud, kd_tree);

    /* Connect Signals. */
    connect(ui->on_start_button_pressed, SIGNAL(pressed()), this, SLOT(on_start_button_pressed()));
    connect(ui->on_stop_button_pressed, SIGNAL(pressed()), this, SLOT(on_stop_button_pressed()));
}

MainWindow::~MainWindow()
{
     delete audio_process;
     delete ui;
}

void MainWindow::on_start_button_pressed()
{
    audio_process->start_stream();
}

void MainWindow::on_stop_button_pressed()
{
    audio_process->stop_stream();
}

} // CATE
