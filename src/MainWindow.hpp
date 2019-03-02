#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QThread>

#include "AudioProcess.hpp"
#include "Plot.hpp"

/* The main Qt application window class. It contains most of the objects and
 * data needed to run the program. */

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
    Ui::MainWindow *ui;
    AudioProcess *audio_process;
    Plot *plot;
    uint16_t sample_rate;
    uint16_t frames_per_buffer;
    uint16_t fft_bin_size;

public slots:
    /* When start button in UI is pressed, plot is re-initialised, and audio
     * stream is started. */
    void on_start_button_pressed();

    /* When stop button in UI is pressed, audio stream is stopped. */
    void on_stop_button_pressed();

    /* Signal is sent from FFT object when frame of FFT data has been
     * calculated. */
    void on_frame_processed(FFT *fft);
};

#endif