#include "ohoaremonitor.h"
#include <QThread>

OHoareMonitor::Condition::Condition():condition(){}
HoareMonitor::Condition *OHoareMonitor::Condition::getCondition(){return &condition;}

OHoareMonitor::OHoareMonitor(): hoareMonitor(){}

void OHoareMonitor::monitorIn(){
    hoareMonitor.monitorIn();
}

void OHoareMonitor::monitorOut(){
    hoareMonitor.monitorOut();
}

void OHoareMonitor::wait(Condition &condition){
    ReadWriteLogger::getInstance()->addWaiting(name);
    hoareMonitor.wait(*(condition.getCondition()));
}
void OHoareMonitor::signal(Condition &condition){
    hoareMonitor.signal(*(condition.getCondition()));
    QThread::currentThread()->currentThreadId();
}

