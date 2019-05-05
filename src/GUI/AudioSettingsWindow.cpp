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

#include <vector>
#include <iomanip>
#include <sstream>

#include "QtUtil.hpp"
#include "AudioSettingsWindow.hpp"
#include "ui_AudioSettingsWindow.h"

using std::vector;

namespace CATE {

AudioSettingsWindow::AudioSettingsWindow(AudioProcess *audio_process, QWidget *parent)
        : QDialog(parent),
          ui(new Ui::AudioSettingsWindow),
          audio_process(audio_process)
{
    ui->setupUi(this);

    populate_boxes();

    connect(ui->sample_rate_box, SIGNAL(currentIndexChanged(int)), this, SLOT(sample_rate_changed(int)));
    connect(ui->buffer_size_box, SIGNAL(currentIndexChanged(int)), this, SLOT(buffer_size_changed(int)));
    connect(ui->bin_size_box, SIGNAL(currentIndexChanged(int)), this, SLOT(bin_size_changed(int)));
    connect(ui->max_grains_box, SIGNAL(currentIndexChanged(int)), this, SLOT(max_grains_changed(int)));
    connect(ui->input_device_box, SIGNAL(currentIndexChanged(int)), this, SLOT(input_device_changed(int)));
    connect(ui->output_device_box, SIGNAL(currentIndexChanged(int)), this, SLOT(output_device_changed(int)));
}


void AudioSettingsWindow::populate_boxes()
{
    const SampleRateVector available_sample_rates = audio_process->get_available_sample_rates();
    const BufferSizeVector available_buffer_sizes = audio_process->get_available_buffer_sizes();
    const BinSizeVector available_bin_sizes = audio_process->get_available_bin_sizes();
    const DeviceList available_audio_devices = audio_process->get_available_devices();
    const MaxGrainsVector available_max_grains = audio_process->get_available_max_grains();

    QStringList sample_rate_list = vector_to_qsl(available_sample_rates);
    QStringList buffer_size_list = vector_to_qsl(available_buffer_sizes);
    QStringList bin_size_list = vector_to_qsl(available_bin_sizes);
    QStringList audio_device_list = vector_to_qsl(available_audio_devices);
    QStringList max_grains_list = vector_to_qsl(available_max_grains);

    ui->sample_rate_box->addItems(sample_rate_list);
    ui->buffer_size_box->addItems(buffer_size_list);
    ui->bin_size_box->addItems(bin_size_list);
    ui->max_grains_box->addItems(max_grains_list);
    ui->input_device_box->addItems(audio_device_list);
    ui->output_device_box->addItems(audio_device_list);

    ui->sample_rate_box->setCurrentIndex(audio_process->get_default_sample_rate_index());
    ui->buffer_size_box->setCurrentIndex(audio_process->get_default_buffer_size_index());
    ui->bin_size_box->setCurrentIndex(audio_process->get_default_bin_size_index());
    ui->max_grains_box->setCurrentIndex(audio_process->get_default_max_grains_index());
    ui->input_device_box->setCurrentIndex(audio_process->get_default_input_device());
    ui->output_device_box->setCurrentIndex(audio_process->get_default_output_device());
}

void AudioSettingsWindow::sample_rate_changed(int selection_index)
{
    audio_process->stop_stream();
    audio_process->set_sample_rate(selection_index);

    if (audio_process->granulator_has_files())
    {
        audio_process->start_stream();
    }
}

void AudioSettingsWindow::buffer_size_changed(int selection_index)
{
    audio_process->stop_stream();
    audio_process->set_buffer_size(selection_index);

    if (audio_process->granulator_has_files())
    {
        audio_process->start_stream();
    }
}

void AudioSettingsWindow::bin_size_changed(int selection_index)
{
    audio_process->stop_stream();
    audio_process->set_bin_size(selection_index);

    if (audio_process->granulator_has_files())
    {
        audio_process->start_stream();
    }
}

void AudioSettingsWindow::input_device_changed(int selection_index)
{
    audio_process->stop_stream();
    audio_process->set_input_device(selection_index);

    if (audio_process->granulator_has_files())
    {
        audio_process->start_stream();
    }
}

void AudioSettingsWindow::output_device_changed(int selection_index)
{
    audio_process->stop_stream();
    audio_process->set_output_device(selection_index);

    if (audio_process->granulator_has_files())
    {
        audio_process->start_stream();
    }
}

void AudioSettingsWindow::max_grains_changed(int selection_index)
{
    audio_process->stop_stream();
    audio_process->set_max_grains(selection_index);
    audio_process->reload_granulator();

    if (audio_process->granulator_has_files())
    {
        audio_process->start_stream();
    }
}

} // CATE
