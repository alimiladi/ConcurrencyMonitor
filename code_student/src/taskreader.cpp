#include "taskreader.h"
#include "stdlib.h"
//#include "abstractreaderwriter.h"
#include "waitinglogger.h"


TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskReader::TaskReader(){}

void TaskReader::run(){

    while(1) {

        SynchroController::getInstance()->pause(id, true, true);
        resource->lockReading();
        ReadWriteLogger::getInstance()->addResourceAccess(QThread::objectName());
        SynchroController::getInstance()->pause(id, true, false);
        resource->unlockReading();
        ReadWriteLogger::getInstance()->removeResourceAccess(QThread::objectName());
    }
}
