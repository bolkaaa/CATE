#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QThread>

#include "AudioProcess.hpp"

/*
  The main Qt application window class. It contains most of the objects and data
  needed to run the program.
 */

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    AudioProcess *audio_process;
    Ui::MainWindow *ui;
    uint16_t sample_rate = 48000;
    uint16_t frames_per_buffer = 256;
    uint16_t fft_bin_size = 1024;

public slots:
    void on_soundcard_settings_action();
    void on_start_button_pressed();
    void on_stop_button_pressed();
    void on_frame_processed();
};

#endif
