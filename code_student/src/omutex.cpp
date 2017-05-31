#include "omutex.h"

OMutex::OMutex()
{
    mutex (QMutex::NonRecursive);//already default mode
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

OMutex::getUnderlyingMutex(){
        return mutex;
    }
