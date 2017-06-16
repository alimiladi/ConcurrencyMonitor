#include "waitinglogger.h"
#include "iostream"
#include <QThread>
#include "reader_writer_thread.h"



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

void WaitingLogger::addWaiting(const QString &objectName)
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
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
    mutex.unlock();
}

void WaitingLogger::removeWaiting(const QString &objectName)
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
    WaitingQueue *queue = contains(objectName);
    if(queue != nullptr){
        queue->removeThreadName(threadName);
        updateView();
    }
    mutex.unlock();
}

QStringList ReadWriteLogger::getResourceAccesses() const
{
    return resourceAccesses;
}

ReadWriteLogger::ReadWriteLogger()
{

}

void ReadWriteLogger::addResourceAccess()
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
    resourceAccesses.append(threadName);
    //on enlève le thread de toutes les listes d'attente car il est dans la ressource
    foreach (WaitingQueue *queue, queues) {
        queue->removeThreadName(threadName);
    }
    updateView();
    mutex.unlock();
}

void ReadWriteLogger::removeResourceAccess()
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
    resourceAccesses.removeOne(threadName);
    updateView();
    mutex.unlock();
}


void ReadWriteLogger::updateView()
{

    unsigned int id = ((ReaderWriterThread*)ReaderWriterThread::currentThread())->getId();
    QString name = ((ReaderWriterThread*)ReaderWriterThread::currentThread())->objectName();

    for(int i = 0; i< queues.size(); i ++){
        logs.append(queues.at(i)->getName()).append(" <- ");
        for(int j =0 ; j< queues.at(i)->getThreadNames().size() ; j ++){
            logs.append(queues.at(i)->getThreadNames().at(j));
            if(j!=queues.at(i)->getThreadNames().size()-1){
                logs.append(" - ");
            }
        }
        logs.append("\n");
    }
    logs.append("In ressource : ");
    for(int k = 0;k <resourceAccesses.size(); k++){
        logs.append(resourceAccesses.at(k));
        if(k != resourceAccesses.size()-1){
            logs.append(", ");
        }
    }


    std::cout << "\n*******************logs de " << qPrintable(name) << "****************\n";
    std::cout << qPrintable(logs);
    std::cout << "\n***************************************************\n" << std::endl;

    //on efface les logs pour le thread courant
    logs.clear();
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
    if(!threadNames.contains(threadName)){
        threadNames.append(threadName);
    }
}

void WaitingQueue::removeThreadName(const QString &threadName){
    threadNames.removeOne(threadName);
}

