#include "ohoaremonitor.h"
#include <QThread>

OHoareMonitor::Condition::Condition():condition(){}

HoareMonitor::Condition *OHoareMonitor::Condition::getCondition(){return &condition;}

void OHoareMonitor::Condition::setName(QString name){
    this->name = name;
}

QString OHoareMonitor::Condition::getName(){
    return name;
}

OHoareMonitor::OHoareMonitor(): hoareMonitor(), mutex(QMutex::NonRecursive), nb_waiting(0), lockedMutex(0){}

void OHoareMonitor::monitorIn(){
    hoareMonitor.monitorIn();
}

void OHoareMonitor::monitorOut(){
    hoareMonitor.monitorOut();
}

void OHoareMonitor::wait(Condition &condition){
    mutex.lock();
    nb_waiting ++;
    ReadWriteLogger::getInstance()->addWaiting(condition.getName());
    mutex.unlock();
    hoareMonitor.wait(*(condition.getCondition()));
    mutex.lock();
    nb_waiting--;
    ReadWriteLogger::getInstance()->removeWaiting(condition.getName());
    mutex.unlock();
    if(nb_waiting > 0)
        lockedMutex.acquire();
}
void OHoareMonitor::signal(Condition &condition){
    mutex.lock();
    if(nb_waiting > 0){
        nb_waiting ++;
        ReadWriteLogger::getInstance()->addWaiting(condition.getName());
        lockedMutex.release();
    }
    mutex.unlock();
    hoareMonitor.signal(*(condition.getCondition()));
}

