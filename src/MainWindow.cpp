#include <QDebug>

#include "AudioProcess.hpp"
#include "Synth.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    audio_process = new AudioProcess(sample_rate, frames_per_buffer,
                                     fft_bin_size);

    connect(ui->on_start_button_pressed, SIGNAL(pressed()), this,
            SLOT(on_start_button_pressed()));

    connect(ui->on_stop_button_pressed, SIGNAL(pressed()), this,
            SLOT(on_stop_button_pressed()));
}

MainWindow::~MainWindow()
{
    delete audio_process;
    delete ui;
}

void MainWindow::on_start_button_pressed()
{
    audio_process->start_stream();
}

void MainWindow::on_stop_button_pressed()
{
    audio_process->stop_stream();
}

void MainWindow::on_soundcard_settings_action()
{
}

void MainWindow::on_frame_processed()
{
}
