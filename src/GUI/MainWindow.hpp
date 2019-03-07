#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QThread>

#include "../Audio/AudioBuffer.hpp"
#include "../Audio/AudioProcess.hpp"
#include "Plot.hpp"

using CATE::AudioProcess;
using CATE::Plot;

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
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui = nullptr;
    AudioProcess *audio_process = nullptr;
    Plot *plot;
    double sample_rate;
    int frames_per_buffer;
    int fft_bin_size;
    vector<float> magspec;

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

} // CATE

#endif
