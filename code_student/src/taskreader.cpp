#include "taskreader.h"
#include "stdlib.h"
#include "abstractreaderwriter.h"


TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

void TaskReader::run(){

    while(1) {
        resource->lockReading();
        std::cout << "Task " << id << " : lecture " << std::endl;
        resource->unlockReading();
    }
}
