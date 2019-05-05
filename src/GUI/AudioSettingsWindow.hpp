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

#ifndef AUDIOSETTINGSWINDOW_HPP
#define AUDIOSETTINGSWINDOW_HPP

#include <QDialog>

#include "src/Synthesis/GrainParams.hpp"
#include "src/Audio/AudioSettings.hpp"
#include "src/Audio/AudioProcess.hpp"

namespace Ui {

/* From .ui file */
class AudioSettingsWindow;

} // Ui

namespace CATE {

class AudioSettingsWindow : public QDialog
{
Q_OBJECT

public:
    AudioSettingsWindow(AudioProcess *audio_process, QWidget *parent = nullptr);

private:
    /* Populate the Qt ComboBox widgets with audio parameter values. */
    void populate_boxes();

    Ui::AudioSettingsWindow *ui;
    AudioProcess *audio_process;

public slots:
    /* Action to take when sample rate value selected from dropdown box. */
    void sample_rate_changed(int selection_index);

    /* Action to take when bin size value selected from dropdown box. */
    void bin_size_changed(int selection_index);

    /* Action to take when buffer size value selected from dropdown box. */
    void buffer_size_changed(int selection_index);

    /* Action to take when max grains value selected from dropdown box. */
    void max_grains_changed(int selection_index);

    /* Action to take when input device value selected from dropdown box. */
    void input_device_changed(int selection_index);

    /* Action to take when output device value selected from dropdown box. */
    void output_device_changed(int selection_index);

};

} // CATE

#endif
