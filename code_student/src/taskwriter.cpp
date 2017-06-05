#include "taskwriter.h"
#include "stdlib.h"
#include "abstractreaderwriter.h"


TaskWriter::TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource){

    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}

TaskWriter::TaskWriter(){}

void TaskWriter::run(){

    while(1) {
        resource->lockReading();
        std::cout << "Task " << id << " : écriture " << std::endl;
        resource->unlockReading();
    }
}
