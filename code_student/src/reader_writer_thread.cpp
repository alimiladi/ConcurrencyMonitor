#include "reader_writer_thread.h"
#include "stdlib.h"
#include "waitinglogger.h"

ReaderWriterThread::ReaderWriterThread(){}
void ReaderWriterThread::run(){}

TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskReader::TaskReader(){}


void TaskReader::run(){

    bool firstTime = true;

    while(1) {

        SynchroController::getInstance()->pause(firstTime);
        firstTime = false;
        resource->lockReading();
        ReadWriteLogger::getInstance()->addResourceAccess();
        SynchroController::getInstance()->pause(firstTime);
        resource->unlockReading();
        ReadWriteLogger::getInstance()->removeResourceAccess();
    }
}


TaskWriter::TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskWriter::TaskWriter(){}

void TaskWriter::run(){

    bool firstTime = true;

    while(1) {
        SynchroController::getInstance()->pause(firstTime);
        firstTime = false;
        resource->lockWriting();
        ReadWriteLogger::getInstance()->addResourceAccess();
        SynchroController::getInstance()->pause(firstTime);
        resource->unlockWriting();
        ReadWriteLogger::getInstance()->removeResourceAccess();
    }
}
