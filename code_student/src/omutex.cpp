#include "omutex.h"

OMutex::OMutex()
{
    mutex (QMutex::NonRecursive);
}

OMutex::~OMutex(){}

OMutex::lock(){
    mutex.lock();
}

OMutex::unlock(){
    mutex.unlock();
}

OMutex::tryLock(){
    mutex.tryLock();
}
