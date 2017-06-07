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

    while(1) {
        SynchroController::getInstance()->pause(id, false, true);
        resource->lockWriting();
        ReadWriteLogger::getInstance()->addResourceAccess(QThread::objectName());
        SynchroController::getInstance()->pause(id, false, false);
        resource->unlockWriting();
        ReadWriteLogger::getInstance()->removeResourceAccess(QThread::objectName());
    }
}
