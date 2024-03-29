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

MainWindow::MainWindow(AudioProcess *audio_process, AudioSettings *audio_settings, Corpus *corpus)
        : ui(new Ui::MainWindow),
          audio_process(audio_process),
          audio_settings(audio_settings),
          corpus(corpus),
          record_worker(new RecordWorker(audio_settings)),
          analysis_worker(new AnalysisWorker(audio_settings, corpus)),
          record_thread(new QThread),
          analysis_thread(new QThread),
          audio_thread(new QThread),
          audio_settings_window(audio_process),
          audio_ready(false)
{
    ui->setupUi(this);

    setup_threads();

    initialise_sliders();

    connect_thread_signals();
    connect_button_signals();
    connect_slider_signals();
    connect_label_signals();
}

void MainWindow::start_playback_button_pressed()
{
    if (!audio_ready)
    {
        audio_status_message();
        return;
    }

    audio_thread->start();
    analysis_thread->start();

    auto error_code = audio_process->start_stream();

    if (!error_code)
    {
        std::cerr << audio_process->report_error(error_code) << "\n";
    }
}

void MainWindow::stop_playback_button_pressed()
{
    audio_process->stop_stream();
}

void MainWindow::start_recording_button_pressed()
{
    if (!audio_ready)
    {
        audio_status_message();
        return;
    }

    record_thread->start();
    audio_process->start_recording();
}

void MainWindow::stop_recording_button_pressed()
{
    if (!audio_ready)
    {
        audio_status_message();
        return;
    }

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

        if (audio_ready)
        {
            audio_process->start_stream();
        }

        return;
    }

    record_worker->save_recording(output_path);

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

    corpus->add_directory(audio_dir_path);
    corpus->sliding_window_analysis(audio_settings->get_bin_size());
    corpus->write_file(corpus_path);
    corpus->rebuild_point_cloud();
    corpus->rebuild_kdtree();
    auto audio_frame_map = corpus->create_audio_frame_map();
    audio_process->load_audio(audio_frame_map);
    audio_ready = true;
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

    corpus->read_file(corpus_path);
    corpus->rebuild_point_cloud();
    corpus->rebuild_kdtree();
    auto audio_frame_map = corpus->create_audio_frame_map();
    audio_process->load_audio(audio_frame_map);
    audio_ready = true;
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

void MainWindow::set_grain_pitch(int new_value)
{
    auto grain_pitch = audio_process->get_grain_pitch();
    auto value = scale(new_value, 0, slider_max, grain_pitch.min, grain_pitch.max);
    audio_process->set_grain_pitch(value);
    update_number_label(ui->grain_pitch_value, value);
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

void MainWindow::set_centroid_label(float *new_value)
{
    update_number_label(ui->centroid_value, *new_value);
}

void MainWindow::set_flatness_label(float *new_value)
{
    update_number_label(ui->flatness_value, *new_value);
}

void MainWindow::set_rolloff_label(float *new_value)
{
    update_number_label(ui->rolloff_value, *new_value);
}

void MainWindow::setup_threads()
{
    record_worker->moveToThread(record_thread);
    analysis_worker->moveToThread(analysis_thread);
    audio_process->moveToThread(audio_thread);
}

void MainWindow::initialise_sliders()
{
    init_slider(audio_process->get_grain_attack(), ui->grain_attack_slider, ui->grain_attack_value);
    init_slider(audio_process->get_grain_sustain(), ui->grain_sustain_slider, ui->grain_sustain_value);
    init_slider(audio_process->get_grain_release(), ui->grain_release_slider, ui->grain_release_value);
    init_slider(audio_process->get_grain_density(), ui->grain_density_slider, ui->grain_density_value);
    init_slider(audio_process->get_grain_size(), ui->grain_size_slider, ui->grain_size_value);
    init_slider(audio_process->get_grain_pitch(), ui->grain_pitch_slider, ui->grain_pitch_value);
}

void MainWindow::connect_thread_signals()
{
    /* Sending input from audio process to analysis worker. */
    connect(audio_process,
            SIGNAL(send_input_data(RingBuffer<float> * )),
            analysis_worker,
            SLOT(input_data_received(RingBuffer<float> * )));

    /* Sending output from audio process to record worker. */
    connect(audio_process,
            SIGNAL(send_output_data(RingBuffer<float> * )),
            record_worker,
            SLOT(output_data_received(RingBuffer<float> * )));

    /* Sending output from analysis worker to audio process. */
    connect(analysis_worker,
            SIGNAL(send_search_results(RingBuffer<int> * )),
            audio_process,
            SLOT(search_results_received(RingBuffer<int> * )));

}

void MainWindow::connect_button_signals()
{
    connect(ui->start_playback, SIGNAL(clicked()), this, SLOT(start_playback_button_pressed()));
    connect(ui->stop_playback, SIGNAL(clicked()), this, SLOT(stop_playback_button_pressed()));
    connect(ui->start_recording, SIGNAL(clicked()), this, SLOT(start_recording_button_pressed()));
    connect(ui->stop_recording, SIGNAL(clicked()), this, SLOT(stop_recording_button_pressed()));
    connect(ui->analyse_directory, SIGNAL(clicked()), this, SLOT(analyse_directory_button_pressed()));
    connect(ui->load_corpus, SIGNAL(clicked()), this, SLOT(load_corpus_button_pressed()));
    connect(ui->audio_settings, SIGNAL(clicked()), this, SLOT(audio_settings_button_pressed()));
}

void MainWindow::connect_slider_signals()
{
    connect(ui->grain_attack_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_attack(int)));
    connect(ui->grain_sustain_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_sustain(int)));
    connect(ui->grain_release_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_release(int)));
    connect(ui->grain_density_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_density(int)));
    connect(ui->grain_size_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_size(int)));
    connect(ui->grain_pitch_slider, SIGNAL(valueChanged(int)), this, SLOT(set_grain_pitch(int)));
}

void MainWindow::connect_label_signals()
{
    connect(analysis_worker,
            SIGNAL(send_centroid(float * )),
            this,
            SLOT(set_centroid_label(float * )));

    connect(analysis_worker,
            SIGNAL(send_flatness(float * )),
            this,
            SLOT(set_flatness_label(float * )));

    connect(analysis_worker,
            SIGNAL(send_rolloff(float * )),
            this,
            SLOT(set_rolloff_label(float * )));
}

void MainWindow::audio_status_message()
{
    QMessageBox msg;
    msg.setText("No files loaded: audio system not ready.");
    msg.exec();
}

} // CATE
