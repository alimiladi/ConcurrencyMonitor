#include "taskwriter.h"
#include "stdlib.h"
#include "waitinglogger.h"


TaskWriter::TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskWriter::TaskWriter(){}

void TaskWriter::run(){

    bool firstTime = true;

    while(1) {
        SynchroController::getInstance()->pause(id, firstTime);
        firstTime = false;
        resource->lockWriting(id);
        ReadWriteLogger::getInstance()->addResourceAccess(id);
        SynchroController::getInstance()->pause(id, firstTime);
        resource->unlockWriting(id);
        ReadWriteLogger::getInstance()->removeResourceAccess(id);
    }
}
