#include <QDebug>

#include "MainWindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Allocation of new QThread and Worker objects happens in constructor. */
    thread = new QThread();
    worker = new Worker();

    worker->moveToThread(thread);
    connect(worker, SIGNAL(valueChanged(QString)), ui->label, SLOT(setText(QString)));
    connect(worker, SIGNAL(workRequested()), thread, SLOT(start()));
    connect(thread, SIGNAL(started()), worker, SLOT(doWork()));
    connect(worker, SIGNAL(finished()), thread, SLOT(quit()), Qt::DirectConnection);
}

MainWindow::~MainWindow()
{
    worker->abort();
    thread->wait();
    qDebug() << "Deleting thread and worker in Thread " << this->QObject::thread()->currentThreadId();
    delete thread;
    delete worker;
    delete ui;
}

void MainWindow::on_startButton_clicked()
{
    /* Previous thread is aborted to avoid having multiple threads running simultaneously. */
    worker->abort();

    /* Will immediately return if thread not runnning. */
    thread->wait();

    /* Request new work from Worker object. */
    worker->requestWork();
}
