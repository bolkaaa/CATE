#include "AudioSettingsWindow.hpp"
#include "ui_AudioSettingsWindow.h"

namespace CATE {

AudioSettingsWindow::AudioSettingsWindow(AudioSettings &audio_settings, QWidget *parent) :
        QDialog(parent),
        ui(new Ui::AudioSettingsWindow),
        audio_settings(audio_settings)
{
    ui->setupUi(this);
}

} // CATE
