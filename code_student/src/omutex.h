#ifndef PMUTEX_H
#define PMUTEX_H

#include <QSemaphore>
#include <QMutex>

#include "waitinglogger.h"


class OMutex
{
public:
    OMutex();

    void lock();

    bool tryLock();

    void unlock();

    void setName(QString name);

    void setThreadName(QString thread_name);


private:
    QMutex mutex;
    QSemaphore sem;
    unsigned int nb_acces;
    QString name, thread_name;

};

#endif // PMUTEX_H
