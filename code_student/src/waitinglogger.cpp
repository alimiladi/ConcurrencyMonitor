#include "waitinglogger.h"

WaitingLogger::WaitingLogger() : queues()
{

}

WaitingLogger *WaitingLogger::getInstance()
{
    static WaitingLogger *instance = new ReadWriteLogger();
    return instance;
}

QList<WaitingQueue *> WaitingLogger::getQueues() const
{
    return queues;
}

void WaitingLogger::updateView()
{

}

QStringList ReadWriteLogger::getResourceAccesses() const
{
    return resourceAccesses;
}

ReadWriteLogger::ReadWriteLogger() : queues()
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
    std::cout << "The current threads trying to access the resource are : \n" << resourceAccesses << std::endl;
}
