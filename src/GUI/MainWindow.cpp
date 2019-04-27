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
#include <QFileDialog>
#include <QInputDialog>
#include <portaudio.h>

#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioProcess.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

namespace CATE {

MainWindow::MainWindow(AudioProcess &audio_process, Corpus &db, PointCloud &point_cloud, KdTree &kd_tree)
        : ui(new Ui::MainWindow),
          audio_process(audio_process),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree)
{
    ui->setupUi(this);

    connect(ui->start_playback, SIGNAL(pressed()), this, SLOT(start_playback_button_pressed()));
    connect(ui->stop_playback, SIGNAL(pressed()), this, SLOT(stop_playback_button_pressed()));
    connect(ui->start_recording, SIGNAL(pressed()), this, SLOT(start_recording_button_pressed()));
    connect(ui->stop_recording, SIGNAL(pressed()), this, SLOT(stop_recording_button_pressed()));
    connect(ui->analyse_directory, SIGNAL(pressed()), this, SLOT(analyse_directory_button_pressed()));
    connect(ui->load_corpus, SIGNAL(pressed()), this, SLOT(load_corpus_button_pressed()));

    connect(ui->grain_amplitude_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_amplitude(int)));
    connect(ui->grain_attack_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_attack(int)));
    connect(ui->grain_release_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_release(int)));
    connect(ui->grain_density_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_density(int)));
    connect(ui->grain_width_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_width(int)));
    connect(ui->grain_size_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_size(int)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_playback_button_pressed()
{
    if (audio_process.is_ready())
    {
        audio_process.start_stream();
    }
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
    audio_process.stop_stream();
    string output_path = save_file_dialog("*.wav");
    audio_process.save_recording(output_path);
    audio_process.start_stream();
}

void MainWindow::analyse_directory_button_pressed()
{
    audio_process.stop_stream();

    string dir_path = directory_dialog();
    if (dir_path.empty())
    {
        return;
    }

    string db_path = save_file_dialog("*.json");
    if (db_path.empty())
    {
        return;
    }

    db.set_json_file(db_path);
    db.add_directory(dir_path);
    db.load_files();
    db.convert_sample_rates(audio_process.get_sample_rate());
    db.sliding_window_analysis(audio_process.get_bin_size(), audio_process.get_frames_per_buffer());
    db.write_json_file();

    rebuild_audio_process();
}

void MainWindow::load_corpus_button_pressed()
{
    string db_path = open_file_dialog("*.json");
    if (db_path.empty())
    {
        return;
    }

    db.set_json_file(db_path);
    db.read_json_data();
    db.load_files();

    rebuild_audio_process();
}


string MainWindow::directory_dialog()
{
    string directory_path = QFileDialog::getExistingDirectory(this,
                                                              tr("Select Directory"),
                                                              "./",
                                                              QFileDialog::ShowDirsOnly |
                                                              QFileDialog::DontResolveSymlinks).toUtf8().constData();


    return directory_path;
}

string MainWindow::save_file_dialog(string file_types)
{
    string file_path = QFileDialog::getSaveFileName(this,
                                                    tr("File Destination"),
                                                    "./",
                                                    tr(file_types.c_str())).toUtf8().constData();

    return file_path;
}

int MainWindow::int_dialog_box(string message, int default_value, int min_value, int max_value, int step_size)
{
    bool ok = true;

    int value = QInputDialog::getInt(this,
                                     tr("Integer"),
                                     tr(message.c_str()),
                                     default_value,
                                     min_value,
                                     max_value,
                                     step_size,
                                     &ok);

    return value;
}

string MainWindow::open_file_dialog(string file_types)
{
    string file_path = QFileDialog::getOpenFileName(this,
                                                    tr("Select File"),
                                                    "./",
                                                    tr(file_types.c_str())).toUtf8().constData();

    return file_path;
}

void MainWindow::rebuild_audio_process()
{
    point_cloud = db.create_point_cloud();
    kd_tree.buildIndex();
    audio_process.reload_granulator();

    if (db.has_data())
    {
        audio_process.enable();
    }
}

void MainWindow::set_grain_amplitude(int new_value)
{
    const float min = 0.0f;
    const float max = 1.0f;
    float amplitude = scale_slider(new_value, min, max);
    audio_process.set_amplitude(amplitude);
    ui->grain_amplitude_value->setText(QString::number(amplitude));
}

void MainWindow::set_grain_attack(int new_value)
{
    const float min = 0.1f;
    const float max = 1.0f;
    float attack = scale_slider(new_value, min, max);
    audio_process.set_grain_attack(attack);
    ui->grain_attack_value->setText(QString::number(attack));
}

void MainWindow::set_grain_release(int new_value)
{
    const float min = 0.1f;
    const float max = 1.0f;
    float release = scale_slider(new_value, min, max);
    audio_process.set_grain_release(release);
    ui->grain_release_value->setText(QString::number(release));
}

void MainWindow::set_grain_density(int new_value)
{
    const float min = 1.0f;
    const float max = 1000.0f;
    float density = scale_slider(new_value, min, max);
    audio_process.set_grain_density(density);
    ui->grain_density_value->setText(QString::number(density));
}

void MainWindow::set_grain_width(int new_value)
{
    const float min = 0.05f;
    const float max = 0.95f;
    float width = scale_slider(new_value, min, max);
    audio_process.set_grain_width(width);
    ui->grain_width_value->setText(QString::number(width));
}

void MainWindow::set_grain_size(int new_value)
{
    const float min = 128;
    const float max = 4096;
    float size = scale_slider(new_value, min, max);
    audio_process.set_grain_size(size);
    ui->grain_size_value->setText(QString::number(size));
}

float MainWindow::scale_slider(int val, float min, float max)
{
    float scaled_val = (max - min) * (static_cast<float>(val) / slider_max) + min;
    return scaled_val;
}



} // CATE
