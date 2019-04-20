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

#include <QMainWindow>
#include <QSlider>

#include "src/Corpus/Corpus.hpp"
#include "src/Corpus/KdTree.hpp"
#include "src/Audio/AudioBuffer.hpp"
#include "src/Audio/AudioProcess.hpp"

using std::string;

/* The main Qt application window class. It contains most of the objects and
 * data needed to run the program. */

namespace Ui {

class MainWindow;

}

namespace CATE {

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    MainWindow(AudioProcess &audio_process, Corpus &db, PointCloud &point_cloud, KdTree &kd_tree);

    ~MainWindow();

private:
    /* Prompt user to select a directory. */
    string directory_dialog();

    /* Prompt user to select a file to open, getting its path. */
    string open_file_dialog(string file_types);

    /* Prompt user to select a file path to save to, getting its path. */
    string save_file_dialog(string file_types);

    /* Prompt user to input an integer value. */
    int int_dialog_box(string message, int default_value, int min_value, int max_value, int step_size);

    /* Convert slider value to a value within a range. */
    float scale_slider(int val, float min, float max);

    /* When database files have changed, recreate data points and reload the granulator. */
    void rebuild_audio_process();

    Ui::MainWindow *ui;
    Corpus &db;
    AudioProcess &audio_process;
    PointCloud &point_cloud;
    KdTree &kd_tree;
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

    /* When load database button in UI is pressed, user selects existing database to analysis data to use. */
    void load_database_button_pressed();

    /* Set grain attack time through slider. */
    void set_grain_attack(int new_value);

    /* Set grain release time through slider. */
    void set_grain_release(int new_value);

    /* Set grain density through slider. */
    void set_grain_density(int new_value);

    /* Set overall output amplitude value through slider. */
    void set_amplitude(int new_value);
};

} // CATE

#endif
