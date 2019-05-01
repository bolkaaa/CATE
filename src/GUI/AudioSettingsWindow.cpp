#include <vector>
#include <iomanip>
#include <sstream>

#include "Util.hpp"
#include "AudioSettingsWindow.hpp"
#include "ui_AudioSettingsWindow.h"

using std::vector;

namespace CATE {

AudioSettingsWindow::AudioSettingsWindow(AudioSettings &audio_settings, AudioProcess &audio_process, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AudioSettingsWindow),
        audio_settings(audio_settings),
        audio_process(audio_process)
{
    ui->setupUi(this);

    populate_boxes();

    connect(ui->sample_rate_box, SIGNAL(currentIndexChanged(int)), this, SLOT(sample_rate_changed(int)));
    connect(ui->buffer_size_box, SIGNAL(currentIndexChanged(int)), this, SLOT(buffer_size_changed(int)));
    connect(ui->bin_size_box, SIGNAL(currentIndexChanged(int)), this, SLOT(bin_size_changed(int)));
    connect(ui->input_device_box, SIGNAL(currentIndexChanged(int)), this, SLOT(input_device_changed(int)));
    connect(ui->output_device_box, SIGNAL(currentIndexChanged(int)), this, SLOT(output_device_changed(int)));
}

void AudioSettingsWindow::populate_boxes()
{
    const vector<float> available_sample_rates = audio_settings.get_available_sample_rates();
    const vector<unsigned long> available_buffer_sizes = audio_settings.get_available_buffer_sizes();
    const vector<int> available_bin_sizes = audio_settings.get_available_bin_sizes();
    const vector<string> available_audio_devices = audio_process.get_available_devices();

    QStringList sample_rate_list = vector_to_qsl(available_sample_rates);
    QStringList buffer_size_list = vector_to_qsl(available_buffer_sizes);
    QStringList bin_size_list = vector_to_qsl(available_bin_sizes);
    QStringList audio_device_list = vector_to_qsl(available_audio_devices);

    ui->sample_rate_box->addItems(sample_rate_list);
    ui->buffer_size_box->addItems(buffer_size_list);
    ui->bin_size_box->addItems(bin_size_list);
    ui->input_device_box->addItems(audio_device_list);
    ui->output_device_box->addItems(audio_device_list);

    ui->sample_rate_box->setCurrentIndex(audio_settings.get_default_sample_rate_index());
    ui->buffer_size_box->setCurrentIndex(audio_settings.get_default_buffer_size_index());
    ui->bin_size_box->setCurrentIndex(audio_settings.get_default_bin_size_index());
    ui->input_device_box->setCurrentIndex(audio_process.get_default_input_device());
    ui->output_device_box->setCurrentIndex(audio_process.get_default_output_device());
}

void AudioSettingsWindow::sample_rate_changed(int selection_index)
{
    audio_process.stop_stream();
    audio_settings.set_sample_rate(selection_index);
    if (audio_process.is_ready())
    {
        audio_process.start_stream();
    }
}

void AudioSettingsWindow::buffer_size_changed(int selection_index)
{
    audio_process.stop_stream();
    audio_settings.set_buffer_size(selection_index);
    if (audio_process.is_ready())
    {
        audio_process.start_stream();
    }
}

void AudioSettingsWindow::bin_size_changed(int selection_index)
{
    audio_process.stop_stream();
    audio_settings.set_bin_size(selection_index);
    if (audio_process.is_ready())
    {
        audio_process.start_stream();
    }
}

void AudioSettingsWindow::input_device_changed(int selection_index)
{
    audio_process.stop_stream();
    audio_process.set_input_device(selection_index);
    if (audio_process.is_ready())
    {
        audio_process.start_stream();
    }
}

void AudioSettingsWindow::output_device_changed(int selection_index)
{
    audio_process.stop_stream();
    audio_process.set_output_device(selection_index);
    if (audio_process.is_ready())
    {
        audio_process.start_stream();
    }
}

} // CATE
