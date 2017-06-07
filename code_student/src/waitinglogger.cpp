#include "waitinglogger.h"
#include "iostream"

WaitingLogger::WaitingLogger() : queues(), mutex(QMutex::NonRecursive)
{

}

WaitingLogger *WaitingLogger::getInstance()
{
    static WaitingLogger *instance = new ReadWriteLogger();
    return instance;
}

WaitingQueue *WaitingLogger::contains(const QString &objectName)
{
    foreach (WaitingQueue* waitingQueue, queues) {
        if (waitingQueue->getName() == objectName) {
            return waitingQueue;
        }
    }
    return nullptr;
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
    WaitingQueue *queue = contains(objectName);
    if(queue != nullptr){
        queue->addThreadName(threadName);
    }
    else{
        queue = new WaitingQueue();
        queue->setName(objectName);
        queue->addThreadName(threadName);
        queues.append(queue);
    }
    updateView();
}

void WaitingLogger::removeWaiting(const QString &threadName, const QString &objectName)
{
    WaitingQueue *queue = contains(objectName);
    if(queue != nullptr){
        queue->removeThreadName(threadName);
        updateView();
    }
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
    updateView();
}

void ReadWriteLogger::removeResourceAccess(const QString &threadName)
{
    resourceAccesses.removeOne(threadName);
    updateView();
}


void ReadWriteLogger::updateView()
{
    mutex.lock();
    QString string;
    for(int i = 0; i< queues.size(); i ++){
        string.append(queues.at(i)->getName()).append(" <- ");
        for(int j =0 ; j< queues.at(i)->getThreadNames().size() ; j ++){
            string.append(queues.at(i)->getThreadNames().at(j));
            if(j!=queues.at(i)->getThreadNames().size()-1){
                string.append("-");
            }
        }
        string.append("\n");
    }
    string.append("In ressource : ");
    for(int k = 0;k <resourceAccesses.size(); k++){
        string.append(resourceAccesses.at(k));
        if(k != resourceAccesses.size()-1){
            string.append(", ");
        }
    }
    string.append("\n");
    std::cout << qPrintable(string) << std::endl;
    mutex.unlock();
}


ReadWriteLogger *ReadWriteLogger::getInstance(){
    return (ReadWriteLogger*)WaitingLogger::getInstance();
}
QString WaitingQueue::getName(){
    return name;
}

QStringList WaitingQueue::getThreadNames(){
    return threadNames;
}

void WaitingQueue::setName(const QString &name){
    this->name = name;
}

void WaitingQueue::addThreadName(const QString &threadName){
    threadNames.append(threadName);
}

void WaitingQueue::removeThreadName(const QString &threadName){
    threadNames.removeOne(threadName);
}

//void ReadWriteLogger::print(QString string){
//    mutex.lock();
//    std::cout << qPrintable(string) << std::endl;
//    mutex.unlock();
//}
