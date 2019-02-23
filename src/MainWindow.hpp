#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QThread>

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

    void set_audio_context()
    {

    }

private:
    Ui::MainWindow *ui;
    QThread *thread;
    Worker *worker;

private slots:
    void on_startButton_clicked();

};

#endif
