#ifndef PSEMAPHORE_H
#define PSEMAPHORE_H

#include <QSemaphore>

class OSemaphore
{
public:
    OSemaphore(int n = 0);

    void acquire();

    void release();

    bool tryAcquire();

private:
    QSemaphore sem;
};

#endif // PSEMAPHORE_H
