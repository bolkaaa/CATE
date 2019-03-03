#include <QDebug>

#include "../Audio/AudioProcess.hpp"
#include "MainWindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      sample_rate(48000),
      frames_per_buffer(256),
      fft_bin_size(1024)
{
    ui->setupUi(this);
    audio_process = new AudioProcess(sample_rate, frames_per_buffer,
                                     fft_bin_size);
    plot = new Plot(fft_bin_size / 2);
    plot->set_appearance("Frequency",
                         "Magnitude",
                         0,
                         static_cast<int>(sample_rate / 2),
                         -120,
                         0,
                         "SansSerif",
                         12);

    /* Insert plot object into UI window. */
    ui->verticalLayout->insertWidget(0, plot->get_plotter());

    /* Connect Signals. */
    connect(ui->on_start_button_pressed, SIGNAL(pressed()), this,
            SLOT(on_start_button_pressed()));

    connect(ui->on_stop_button_pressed, SIGNAL(pressed()), this,
            SLOT(on_stop_button_pressed()));

    connect(audio_process, SIGNAL(frame_processed(FFT*)), this,
            SLOT(on_frame_processed(FFT*)));
}

MainWindow::~MainWindow()
{
    delete plot;
    delete audio_process;
    delete ui;
}

void MainWindow::on_start_button_pressed()
{
    auto n = fft_bin_size / 2 + 1;

    for (std::size_t i = 0; i < n; ++i)
    {
        double x = (static_cast<double> (i) / n) * (sample_rate / 2.0);
        plot->set_x_data(i, x);
    }

    plot->set_axis_scale(0, static_cast<int>(sample_rate / 2), -120, 0);

    audio_process->start_stream();
}

void MainWindow::on_stop_button_pressed()
{
    audio_process->stop_stream();
}

void MainWindow::on_frame_processed(FFT *fft)
{
    auto n = fft_bin_size / 2 + 1;

    for (auto i = 0; i < n; ++i)
    {
        /* Convert to dB. */
        double y = 20.0 * std::log10((1.0 / n) * fft->magspec[i]);
        plot->set_y_data(i, y);
    }

    plot->replot();
}
