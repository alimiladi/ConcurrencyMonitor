#include "owaitcondition.h"

OWaitCondition::OWaitCondition() :
    cond(),
    mutex(QMutex::NonRecursive){}


bool OWaitCondition::wait(OMutex *mutex){
    cond.wait(mutex->getUnderlyingMutex());
}

void OWaitCondition::wakeOne(){
    cond.wakeOne();
}

void OWaitCondition::wakeAll(){
    cond.wakeAll();
}
