#include "omutex.h"

OMutex::OMutex() : mutex(QMutex::NonRecursive) , prevMutex(QMutex::NonRecursive)
{

}

void OMutex::lock(){
    mutex.lock();
}

void OMutex::unlock(){
    mutex.unlock();
}

bool OMutex::tryLock(){
    mutex.tryLock();
}

QMutex *OMutex::getUnderlyingMutex(){
        return &mutex;
    }
