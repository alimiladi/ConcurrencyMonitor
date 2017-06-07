#include "taskwriter.h"
#include "stdlib.h"
//#include "abstractreaderwriter.h"


TaskWriter::TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource, SynchroController* synchroController){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
    this->synchroController = synchroController;
}

TaskWriter::TaskWriter(){}

void TaskWriter::run(){

    while(1) {
        resource->lockReading();
        std::cout << "Task " << id << " : écriture " << std::endl;
        synchroController->pause();
        resource->unlockReading();
    }
}
