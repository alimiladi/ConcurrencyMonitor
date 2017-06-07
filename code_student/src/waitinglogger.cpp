#include "waitinglogger.h"

WaitingLogger::WaitingLogger() : queues()
{

}

WaitingLogger *WaitingLogger::getInstance()
{
    static WaitingLogger *instance = new ReadWriteLogger();
    return instance;
}

bool WaitingLogger::contains(QString &objectName)
{
    foreach (WaitingQueue* waitingQueue, queues) {
        if (waitingQueue->getName() == objectName) {
            return true;
        }
        return false;
    }
}

QList<WaitingQueue *> WaitingLogger::getQueues() const
{
    return queues;
}

void WaitingLogger::updateView()
{

}

void WaitingLogger::addWaiting(const QString &threadName, const QString &objectName)
{
//    if (contains(objectName)) {
//        queues.
//    }
}

void WaitingLogger::removeWaiting(const QString &threadName, const QString &objectName)
{

}

QStringList ReadWriteLogger::getResourceAccesses() const
{
    return resourceAccesses;
}

ReadWriteLogger::ReadWriteLogger()
{

}

void ReadWriteLogger::addResourceAccess(const QString &threadName)
{
    resourceAccesses.append(threadName);
}

void ReadWriteLogger::removeResourceAccess(const QString &threadName)
{
    resourceAccesses.removeOne(threadName);
}


void ReadWriteLogger::updateView()
{
    //    std::cout << "The current threads trying to access the resource are " << resourceAccesses << std::endl;
}
