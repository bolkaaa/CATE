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

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <memory>

#include <QMainWindow>
#include <QSlider>
#include <QString>
#include <QLabel>
#include <QEvent>

#include "src/Corpus/Corpus.hpp"
#include "src/Corpus/KdTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioProcess.hpp"
#include "src/Audio/RecordWorker.hpp"
#include "src/Audio/AnalysisWorker.hpp"
#include "AudioSettingsWindow.hpp"

using std::string;
using std::unique_ptr;

namespace Ui {

/* From .ui file */
class MainWindow;

} // Ui

namespace CATE {

/* The main Qt application window class. It contains most of the objects and
 * data needed to run the program. */
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(AudioProcess *audio_process, AudioSettings *audio_settings, Corpus *corpus);

private:
    /* Prompt user to select a directory. */
    string directory_dialog();

    /* Prompt user to select a file to open, getting its path. */
    string open_file_dialog(const string &file_types);

    /* Prompt user to select a file path to save to, getting its path. */
    string save_file_dialog(const string &file_types);

    /* Convert value from an input range to an output range. */
    float scale(float input, float input_min, float input_max, float output_min, float output_max);

    /* Return the path of the user's home directory (cross-platform). */
    QString get_home_dir_path();

    /* Overriding Qt closeEvent function to define behaviour for when window is closed. */
    void closeEvent(QCloseEvent *event) override;

    /* Initialise slider with defaults from parameter. */
    template <class T>
    void init_slider(Param<T> param, QSlider *slider, QLabel *label)
    {
        slider->setMaximum(slider_max);
        auto slider_value = scale(param.start, param.min, param.max, 0, slider_max);
        slider->setValue(static_cast<int>(slider_value));
        update_number_label(label, param.start);
    }

    Ui::MainWindow *ui;
    AudioSettingsWindow audio_settings_window;
    AudioProcess *audio_process;
    AudioSettings *audio_settings;
    Corpus *corpus;
    QThread *record_thread;
    QThread *analysis_thread;
    RecordWorker *record_worker;
    AnalysisWorker *analysis_worker;
    const int slider_max = 127;

public slots:
    /* When playback start button in UI is pressed, audio stream is started. */
    void start_playback_button_pressed();

    /* When playback stop button in UI is pressed, audio stream is stopped. */
    void stop_playback_button_pressed();

    /* When recording start button in UI is pressed, current audio output begins recording. */
    void start_recording_button_pressed();

    /* When recording stop button in UI is pressed, current audio output stops recording. */
    void stop_recording_button_pressed();

    /* When analyse directory button in UI is pressed, user selects directory of audio files to analyse. */
    void analyse_directory_button_pressed();

    /* When load corpus button in UI is pressed, user selects existing corpus data to use. */
    void load_corpus_button_pressed();

    /* When audio settings button in UI is pressed, dialog box allows setting audio parameters. */
    void audio_settings_button_pressed();

    /* Set grain attack time through slider. */
    void set_grain_attack(int new_value);

    /* Set grain release time through slider. */
    void set_grain_release(int new_value);

    /* Set grain density through slider. */
    void set_grain_density(int new_value);

    /* Set grain amplitude value through slider. */
    void set_grain_sustain(int new_value);

    /* Set grain size value through slider. */
    void set_grain_size(int new_value);
};

} // CATE

#endif
