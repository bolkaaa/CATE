#ifndef THREAD_TEST_HPP
#define THREAD_TEST_HPP

/*
  Based on demo program at https://github.com/fabienpn/simple-qt-thread-example
 */

#include <QObject>

#include "AudioParameters.hpp"
#include "Synth.hpp"

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject *parent = 0);

    void requestWork();

    void abort();

private:
    bool _abort;
    bool _working;
    QMutex mutex;

signals:
    void workRequested();

    void valueChanged(const QString &value);

    void finished();

public slots:
    void doWork();

};

#endif
