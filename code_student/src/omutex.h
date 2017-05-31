#ifndef PMUTEX_H
#define PMUTEX_H

#include <QMutex>


class OMutex
{
public:
    OMutex();

    void lock();

    bool tryLock();

    void unlock();

    QMutex getUnderlyingMutex();

private:
    QMutex mutex;
    QMutex prevMutex;

};

#endif // PMUTEX_H
