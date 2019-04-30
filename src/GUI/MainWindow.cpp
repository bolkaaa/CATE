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
#include <QStandardPaths>

#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioProcess.hpp"

namespace CATE {

MainWindow::MainWindow(AudioProcess &audio_process, AudioSettings &audio_settings, Corpus &db, PointCloud &point_cloud,
                       KdTree &kd_tree)
        : ui(new Ui::MainWindow),
          audio_settings(audio_settings),
          audio_settings_window(audio_settings, nullptr),
          audio_process(audio_process),
          db(db),
          point_cloud(point_cloud),
          kd_tree(kd_tree)
{
    ui->setupUi(this);

    connect(ui->start_playback, SIGNAL(clicked()), this, SLOT(start_playback_button_pressed()));
    connect(ui->stop_playback, SIGNAL(clicked()), this, SLOT(stop_playback_button_pressed()));
    connect(ui->start_recording, SIGNAL(clicked()), this, SLOT(start_recording_button_pressed()));
    connect(ui->stop_recording, SIGNAL(clicked()), this, SLOT(stop_recording_button_pressed()));
    connect(ui->analyse_directory, SIGNAL(clicked()), this, SLOT(analyse_directory_button_pressed()));
    connect(ui->load_corpus, SIGNAL(clicked()), this, SLOT(load_corpus_button_pressed()));
    connect(ui->audio_settings, SIGNAL(clicked()), this, SLOT(audio_settings_button_pressed()));

    connect(ui->grain_amplitude_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_amplitude(int)));
    connect(ui->grain_attack_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_attack(int)));
    connect(ui->grain_release_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_release(int)));
    connect(ui->grain_density_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_density(int)));
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
    if (output_path.empty())
    {
        if (audio_process.is_ready())
        {
            audio_process.start_stream();
        }

        return;
    }

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
    db.sliding_window_analysis();
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
    QString directory_path = QFileDialog::getExistingDirectory(this,
                                                               tr("Select Directory"),
                                                               get_home_dir_path(),
                                                               QFileDialog::ShowDirsOnly |
                                                               QFileDialog::DontResolveSymlinks);


    return qstring_to_string(directory_path);
}

string MainWindow::save_file_dialog(string file_types)
{
    QString file_path = QFileDialog::getSaveFileName(this,
                                                     tr("File Destination"),
                                                     get_home_dir_path(),
                                                     tr(file_types.c_str()));

    return qstring_to_string(file_path);
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
    QString file_path = QFileDialog::getOpenFileName(this,
                                                     tr("Select File"),
                                                     get_home_dir_path(),
                                                     tr(file_types.c_str()));

    return qstring_to_string(file_path);
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
    const int min = 1;
    const int max = 512;
    int density = static_cast<int>(scale_slider(new_value, min, max));
    audio_process.set_grain_density(density);
    ui->grain_density_value->setText(QString::number(density));
}

void MainWindow::set_grain_size(int new_value)
{
    const int min = 64;
    const int max = 1024;
    int size = static_cast<int>(scale_slider(new_value, min, max));
    audio_process.set_grain_size(size);
    ui->grain_size_value->setText(QString::number(size));
}


float MainWindow::scale_slider(int val, float min, float max)
{
    float scaled_val = (max - min) * (static_cast<float>(val) / slider_max) + min;
    return scaled_val;
}

QString MainWindow::string_to_qstring(const std::string &str)
{
    QString qstr = QString::fromUtf8(str.c_str());
    return qstr;
}

string MainWindow::qstring_to_string(const QString &qstr)
{
    string str = qstr.toUtf8().constData();
    return str;
}

QString MainWindow::get_home_dir_path()
{
    QString home_dir_path = QStandardPaths::locate(QStandardPaths::HomeLocation,
                                                   QString(),
                                                   QStandardPaths::LocateDirectory);

    return home_dir_path;
}

void MainWindow::audio_settings_button_pressed()
{
    audio_settings_window.exec();
}

} // CATE
