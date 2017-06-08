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

    void setName(QString name);

    void setThreadName (QString thread_name);

private:

    QWaitCondition cond;
    QString name, thread_name;
    QMutex mutex;
};

#endif // PWAITCONDITION_H
