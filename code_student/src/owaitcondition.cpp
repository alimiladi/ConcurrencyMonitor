#include "owaitcondition.h"

OWaitCondition::OWaitCondition()
{
    cond();
    mutex(QMutex::NonRecursive);
}

OWaitCondition::~OWaitCondition(){}

OWaitCondition::wait(OMutex *mutex){
    cond.wait(&mutex->getUnderlyingMutex());
}

OWaitCondition::wakeOne(){
    cond.wakeOne();
}

OWaitCondition::wakeAll(){
    cond.wakeAll();
}
