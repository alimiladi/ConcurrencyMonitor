#include "taskreader.h"
#include "stdlib.h"
//#include "abstractreaderwriter.h"


TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource, SynchroController* synchroController){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
    this->synchroController = synchroController;
}

TaskReader::TaskReader(){}

void TaskReader::run(){

    while(1) {
        resource->lockReading();
        std::cout << "Task " << id << " : lecture " << std::endl;
        synchroController->pause();
        resource->unlockReading();
    }
}
