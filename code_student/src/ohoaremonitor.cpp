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

OHoareMonitor::OHoareMonitor(): hoareMonitor(){}

void OHoareMonitor::monitorIn(){
    hoareMonitor.monitorIn();
}

void OHoareMonitor::monitorOut(){
    hoareMonitor.monitorOut();
}

void OHoareMonitor::wait(Condition &condition){
    ReadWriteLogger::getInstance()->addWaiting(condition.getName());
    hoareMonitor.wait(*(condition.getCondition()));
    ReadWriteLogger::getInstance()->removeWaiting(condition.getName());
}
void OHoareMonitor::signal(Condition &condition){
    hoareMonitor.signal(*(condition.getCondition()));
}

