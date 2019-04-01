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

#include "../Database/Database.hpp"
#include "../Database/KdTree.hpp"
#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioProcess.hpp"

/* The main Qt application window class. It contains most of the objects and
 * data needed to run the program. */

namespace Ui
{
    class MainWindow;
}

namespace CATE {

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent, AudioProcess &audio_process);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    AudioProcess &audio_process;

public slots:
    /* When playback start button in UI is pressed, audio stream is started. */
    void start_playback_button_pressed();

    /* When playback stop button in UI is pressed, audio stream is stopped. */
    void stop_playback_button_pressed();

    /* When recording start button in UI is pressed, current audio output begins recording. */
    void start_recording_button_pressed();

    /* When recording stop button in UI is pressed, current audio output stops recording. */
    void stop_recording_button_pressed();

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
