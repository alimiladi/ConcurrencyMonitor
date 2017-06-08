#include "taskreader.h"
#include "stdlib.h"
#include "waitinglogger.h"


TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskReader::TaskReader(){}


void TaskReader::run(){

    bool firstTime = true;

    while(1) {

        SynchroController::getInstance()->pause(id, firstTime);
        firstTime = false;
        resource->lockReading(id);
        ReadWriteLogger::getInstance()->addResourceAccess(id);
        SynchroController::getInstance()->pause(id, firstTime);
        resource->unlockReading(id);
        ReadWriteLogger::getInstance()->removeResourceAccess(id);
    }
}
