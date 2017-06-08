#include "waitinglogger.h"
#include "iostream"
#include <QThread>

void WaitingLogger::printLogs(unsigned int id){
    std::cout << qPrintable(*(this->logs->at(id))) << std::endl;
}

void WaitingLogger::clearLogs(unsigned int id){
    this->logs->at(id)->clear();
}


void WaitingLogger::setSizeLogs(unsigned int nbThreads){

    for(int i = 0; i < nbThreads; i++){
        logs->append(new QString(""));
    }
}


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

void WaitingLogger::updateView(unsigned int id)
{

}

void WaitingLogger::addWaiting(const QString &objectName, unsigned int id)
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
    updateView(id);
    mutex.unlock();
}

void WaitingLogger::removeWaiting(const QString &objectName, unsigned int id)
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
    WaitingQueue *queue = contains(objectName);
    if(queue != nullptr){
        queue->removeThreadName(threadName);
        updateView(id);
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

void ReadWriteLogger::addResourceAccess(unsigned int id)
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
    resourceAccesses.append(threadName);
    //on enlève le thread de toutes les listes d'attente car il est dans la ressource
    foreach (WaitingQueue *queue, queues) {
        queue->removeThreadName(threadName);
    }
    updateView(id);
    mutex.unlock();
}

void ReadWriteLogger::removeResourceAccess(unsigned int id)
{
    mutex.lock();
    QString threadName = QThread::currentThread()->objectName();
    resourceAccesses.removeOne(threadName);
    updateView(id);
    mutex.unlock();
}


void ReadWriteLogger::updateView(unsigned int id)
{

    //avant que le thread écrive ses logs,
    //il doit effacer les précédents car on sait qu'ils ont déjà été affichés.
    logs->at(id)->clear();

    logs->at(id)->append("\n***************************************************\n");
    for(int i = 0; i< queues.size(); i ++){
        logs->at(id)->append(queues.at(i)->getName()).append(" <- ");
        for(int j =0 ; j< queues.at(i)->getThreadNames().size() ; j ++){
            logs->at(id)->append(queues.at(i)->getThreadNames().at(j));
            if(j!=queues.at(i)->getThreadNames().size()-1){
                logs->at(id)->append(" - ");
            }
        }
        logs->at(id)->append("\n");
    }
    logs->at(id)->append("In ressource : ");
    for(int k = 0;k <resourceAccesses.size(); k++){
        logs->at(id)->append(resourceAccesses.at(k));
        if(k != resourceAccesses.size()-1){
            logs->at(id)->append(", ");
        }
    }
    logs->at(id)->append("\n***************************************************\n");


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
