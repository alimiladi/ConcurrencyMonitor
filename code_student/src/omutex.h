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

    QMutex getUnderlyingMutex(){
        return mutex;
    }

private:
    QMutex mutex;
};

#endif // PMUTEX_H
