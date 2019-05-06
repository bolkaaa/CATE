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

#include "QtUtil.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/RingBuffer.hpp"
#include "src/Audio/AudioProcess.hpp"

using std::make_unique;

namespace CATE {

MainWindow::MainWindow(AudioProcess *audio_process, AudioSettings *audio_settings)
        : ui(new Ui::MainWindow),
          audio_process(audio_process),
          record_worker(new RecordWorker),
          analysis_worker(new AnalysisWorker(audio_settings)),
          record_thread(new QThread),
          analysis_thread(new QThread),
          audio_settings_window(audio_process)
{
    ui->setupUi(this);

    record_worker->moveToThread(record_thread);
    analysis_worker->moveToThread(analysis_thread);

    init_slider(audio_process->get_grain_attack(), ui->grain_attack_slider, ui->grain_attack_value);
    init_slider(audio_process->get_grain_sustain(), ui->grain_sustain_slider, ui->grain_sustain_value);
    init_slider(audio_process->get_grain_release(), ui->grain_release_slider, ui->grain_release_value);
    init_slider(audio_process->get_grain_density(), ui->grain_density_slider, ui->grain_density_value);
    init_slider(audio_process->get_grain_size(), ui->grain_size_slider, ui->grain_size_value);

    /* Connect signals and slots. */
    connect(audio_process,
            SIGNAL(send_input_data(RingBuffer * )),
            analysis_worker,
            SLOT(input_data_received(RingBuffer * )));

    connect(audio_process,
            SIGNAL(send_output_data(RingBuffer * )),
            record_worker,
            SLOT(output_data_received(RingBuffer * )));

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

void MainWindow::start_playback_button_pressed()
{
    auto error_code = 0;

    // if (audio_process->granulator_has_files())
    // {

    error_code = audio_process->start_stream();

    if (!error_code)
    {
        std::cerr << audio_process->report_error(error_code) << "\n";
    }

    analysis_thread->start();

    //}
//    else
//    {
//        QMessageBox msg;
//        msg.setText("No files loaded.");
//        msg.exec();
//    }
}

void MainWindow::stop_playback_button_pressed()
{
    audio_process->stop_stream();
}

void MainWindow::start_recording_button_pressed()
{
    audio_process->start_recording();
    record_thread->start();
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

        if (audio_process->granulator_has_files())
        {
            audio_process->start_stream();
        }

        return;
    }

    record_worker->save_recording(output_path,
                                  audio_process->get_sample_rate().value,
                                  audio_process->get_num_output_channels());

    audio_process->start_stream();
}

void MainWindow::analyse_directory_button_pressed()
{
    audio_process->stop_stream();

    auto audio_dir_path = string();
    auto corpus_path = string();

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

    audio_process->analyse_directory(audio_dir_path);
}

void MainWindow::load_corpus_button_pressed()
{
    auto corpus_path = string();

    try
    {
        corpus_path = open_file_dialog("*.json");
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << e.what() << "\n";
        return;
    }

    audio_process->load_corpus(corpus_path);
}


string MainWindow::directory_dialog()
{
    auto directory_path = qstring_to_string(QFileDialog::getExistingDirectory(this,
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
    auto file_path = qstring_to_string(QFileDialog::getSaveFileName(this,
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
    auto file_path = qstring_to_string(QFileDialog::getOpenFileName(this,
                                                                      tr("Select File"),
                                                                      get_home_dir_path(),
                                                                      tr(file_types.c_str())));

    if (file_path.empty())
    {
        throw std::invalid_argument("Empty file path when opening file.");
    }

    return file_path;
}

void MainWindow::set_grain_attack(int new_value)
{
    auto grain_attack = audio_process->get_grain_attack();
    auto value = scale(new_value, 0, slider_max, grain_attack.min, grain_attack.max);
    audio_process->set_grain_attack(value);
    update_number_label(ui->grain_attack_value, value);
}

void MainWindow::set_grain_sustain(int new_value)
{
    auto grain_sustain = audio_process->get_grain_sustain();
    auto value = scale(new_value, 0, slider_max, grain_sustain.min, grain_sustain.max);
    audio_process->set_grain_sustain(value);
    update_number_label(ui->grain_sustain_value, value);
}

void MainWindow::set_grain_release(int new_value)
{
    auto grain_release = audio_process->get_grain_release();
    auto value = scale(new_value, 0, slider_max, grain_release.min, grain_release.max);
    audio_process->set_grain_release(value);
    update_number_label(ui->grain_release_value, value);
}

void MainWindow::set_grain_density(int new_value)
{
    auto grain_density = audio_process->get_grain_density();
    auto value = scale(new_value, 0, slider_max, grain_density.min, grain_density.max);
    audio_process->set_grain_density(value);
    update_number_label(ui->grain_density_value, value);
}

void MainWindow::set_grain_size(int new_value)
{
    auto grain_size = audio_process->get_grain_size();
    auto value = scale(new_value, 0, slider_max, grain_size.min, grain_size.max);
    audio_process->set_grain_size(value);
    update_number_label(ui->grain_size_value, value);
}

float MainWindow::scale(float input, float input_min, float input_max, float output_min, float output_max)
{
    auto output = (((input - input_min) * (output_max - output_min)) / (input_max - input_min)) + output_min;
    return output;
}

QString MainWindow::get_home_dir_path()
{
    auto home_dir_path = QStandardPaths::locate(QStandardPaths::HomeLocation,
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
