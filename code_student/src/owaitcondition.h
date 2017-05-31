#ifndef PWAITCONDITION_H
#define PWAITCONDITION_H

#include "omutex.h"
#include <QWaitCondition>

class OWaitCondition
{
public:
    OWaitCondition();

    bool wait(OMutex *lockedMutex);

    void wakeAll();

    void wakeOne();

private:
    QWaitCondition cond;
    QMutex mutex;

};

#endif // PWAITCONDITION_H
