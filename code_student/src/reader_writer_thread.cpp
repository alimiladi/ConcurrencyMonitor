#include "reader_writer_thread.h"
#include "stdlib.h"
#include "waitinglogger.h"

ReaderWriterThread::ReaderWriterThread(){

    synchroController = SynchroController::getInstance();
    waitingLogger = ReadWriteLogger::getInstance();
}
void ReaderWriterThread::run(){}


TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    //ReaderWriterThread();
    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskReader::TaskReader(){}


void TaskReader::run(){


    while(1) {

        synchroController->pause();
        setFirstTime(false);
        resource->lockReading();
        waitingLogger->addResourceAccess();
        synchroController->pause();
        resource->unlockReading();
        waitingLogger->removeResourceAccess();
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
        synchroController->pause();
        setFirstTime(false);
        resource->lockWriting();
        waitingLogger->addResourceAccess();
        synchroController->pause();
        waitingLogger->removeResourceAccess();
        resource->unlockWriting();
    }
}
