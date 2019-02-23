#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QThread>

#include "AudioProcess.hpp"
#include "Synth.hpp"
#include "Worker.hpp"

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

public slots:
    void on_soundcard_settings_action();
    void on_start_button_pressed();
    void on_stop_button_pressed();
    void on_frame_processed();

private slots:
};

#endif
