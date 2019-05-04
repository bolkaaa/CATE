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
#include <QtWidgets>

#include "Util.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioProcess.hpp"

namespace CATE {

MainWindow::MainWindow(const unique_ptr<AudioSettings> &audio_settings, const unique_ptr<AudioProcess> &audio_process,
                       const unique_ptr<Corpus> &db, const unique_ptr<PointCloud> &point_cloud,
                       const unique_ptr<GrainParams> &grain_params, const unique_ptr<EnvelopeParams> &env_params,
                       KdTree &kd_tree)
        : ui(new Ui::MainWindow),
          audio_settings(audio_settings.get()),
          audio_process(audio_process.get()),
          corpus(db.get()),
          point_cloud(point_cloud.get()),
          grain_params(grain_params.get()),
          env_params(env_params.get()),
          kd_tree(kd_tree),
          audio_settings_window(audio_settings.get(), audio_process.get(), grain_params.get())
{
    ui->setupUi(this);

    ui->grain_attack_slider->setMaximum(slider_resolution);
    ui->grain_sustain_slider->setMaximum(slider_resolution);
    ui->grain_release_slider->setMaximum(slider_resolution);
    ui->grain_density_slider->setMaximum(slider_resolution);
    ui->grain_size_slider->setMaximum(slider_resolution);

    connect(ui->start_playback, SIGNAL(clicked()), this, SLOT(start_playback_button_pressed()));
    connect(ui->stop_playback, SIGNAL(clicked()), this, SLOT(stop_playback_button_pressed()));
    connect(ui->start_recording, SIGNAL(clicked()), this, SLOT(start_recording_button_pressed()));
    connect(ui->stop_recording, SIGNAL(clicked()), this, SLOT(stop_recording_button_pressed()));
    connect(ui->analyse_directory, SIGNAL(clicked()), this, SLOT(analyse_directory_button_pressed()));
    connect(ui->load_corpus, SIGNAL(clicked()), this, SLOT(load_corpus_button_pressed()));
    connect(ui->audio_settings, SIGNAL(clicked()), this, SLOT(audio_settings_button_pressed()));

    connect(ui->grain_attack_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_attack(int)));
    connect(ui->grain_sustain_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_sustain(int)));
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
    int error_code = 0;

    if (audio_process->is_ready())
    {
        error_code = audio_process->start_stream();

        if (!error_code)
        {
            std::cerr << audio_process->report_error(error_code) << "\n";
        }
    } else
    {
        QMessageBox msg;
        msg.setText("No files loaded.");
        msg.exec();
    }
}

void MainWindow::stop_playback_button_pressed()
{
    audio_process->stop_stream();
}

void MainWindow::start_recording_button_pressed()
{
    audio_process->start_recording();
}

void MainWindow::stop_recording_button_pressed()
{
    audio_process->stop_recording();
    audio_process->stop_stream();
    string output_path;

    try
    {
        output_path = save_file_dialog("*.wav");
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << "\n";

        if (audio_process->is_ready())
        {
            audio_process->start_stream();
        }

        return;
    }

    audio_process->save_recording(output_path);
    audio_process->start_stream();
}

void MainWindow::analyse_directory_button_pressed()
{
    audio_process->stop_stream();
    string audio_dir_path;
    string corpus_path;

    try
    {
        audio_dir_path = directory_dialog();
        corpus_path = save_file_dialog("*.json");
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << "\n";
        return;
    }

    corpus->add_directory(audio_dir_path);
    corpus->sliding_window_analysis();
    corpus->write_file(corpus_path);

    rebuild_audio_process();
}

void MainWindow::load_corpus_button_pressed()
{
    string corpus_path;

    try
    {
        corpus_path = open_file_dialog("*.json");
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << "\n";
        return;
    }

    corpus->read_file(corpus_path);
    corpus->load_audio_from_db();

    rebuild_audio_process();
}


string MainWindow::directory_dialog()
{
    string directory_path = qstring_to_string(QFileDialog::getExistingDirectory(this,
                                                                                tr("Select Directory"),
                                                                                get_home_dir_path(),
                                                                                QFileDialog::ShowDirsOnly |
                                                                                QFileDialog::DontResolveSymlinks));

    if (directory_path.empty())
    {
        throw std::invalid_argument("Empty directory path when selecting directory.");
    }

    return directory_path;
}

string MainWindow::save_file_dialog(const string &file_types)
{
    string file_path = qstring_to_string(QFileDialog::getSaveFileName(this,
                                                                      tr("File Destination"),
                                                                      get_home_dir_path(),
                                                                      tr(file_types.c_str())));

    if (file_path.empty())
    {
        throw std::invalid_argument("Empty file path when saving file.");
    }

    return file_path;
}

string MainWindow::open_file_dialog(const string &file_types)
{
    string file_path = qstring_to_string(QFileDialog::getOpenFileName(this,
                                                                      tr("Select File"),
                                                                      get_home_dir_path(),
                                                                      tr(file_types.c_str())));

    if (file_path.empty())
    {
        throw std::invalid_argument("Empty file path when opening file.");
    }

    return file_path;
}

void MainWindow::rebuild_audio_process()
{
    corpus->rebuild_point_cloud();
    kd_tree.buildIndex();
    audio_process->reload_granulator();

    if (corpus->has_data())
    {
        audio_process->enable();
    }
}

void MainWindow::set_grain_sustain(int new_value)
{
    const float min = 0.00f;
    const float max = 1.0f;
    float sustain = scale_slider(new_value, min, max);
    audio_process->set_grain_sustain(sustain);
    update_number_label(ui->grain_sustain_value, sustain);
}

void MainWindow::set_grain_attack(int new_value)
{
    const float min = 0.1f;
    const float max = 1.0f;
    float attack = scale_slider(new_value, min, max);
    audio_process->set_grain_attack(attack);
    update_number_label(ui->grain_attack_value, attack);
}

void MainWindow::set_grain_release(int new_value)
{
    const float min = 0.1f;
    const float max = 1.0f;
    float release = scale_slider(new_value, min, max);
    audio_process->set_grain_release(release);
    update_number_label(ui->grain_release_value, release);
}

void MainWindow::set_grain_density(int new_value)
{
    const int min = 1;
    const int max = 512;
    int density = static_cast<int>(scale_slider(new_value, min, max));
    audio_process->set_grain_density(density);
    update_number_label(ui->grain_density_value, density);
}

void MainWindow::set_grain_size(int new_value)
{
    const int min = 32;
    const int max = 256;
    int grain_size = static_cast<int>(scale_slider(new_value, min, max));
    audio_process->set_grain_size(grain_size);
    update_number_label(ui->grain_size_value, grain_size);
}

float MainWindow::scale_slider(int val, float min, float max)
{
    float scaled_val = (max - min) * (static_cast<float>(val) / slider_resolution) + min;
    return scaled_val;
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

void MainWindow::closeEvent(QCloseEvent *event)
{
    audio_process->stop_stream();
}


} // CATE
