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


    while(1) {

        SynchroController::getInstance()->pause();
        setFirstTime(false);
        resource->lockReading();
        ReadWriteLogger::getInstance()->addResourceAccess();
        SynchroController::getInstance()->pause();
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

    while(1) {
        SynchroController::getInstance()->pause();
        setFirstTime(false);
        resource->lockWriting();
        ReadWriteLogger::getInstance()->addResourceAccess();
        SynchroController::getInstance()->pause();
        ReadWriteLogger::getInstance()->removeResourceAccess();
        resource->unlockWriting();
    }
}
