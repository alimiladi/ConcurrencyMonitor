#include "owaitcondition.h"

OWaitCondition::OWaitCondition() : cond(), mutex(QMutex::NonRecursive){}

void OWaitCondition::setName(QString name){this->name = name;}

bool OWaitCondition::wait(OMutex *lockedMutex){
    mutex.lock(); // mutex sur lequel la QWaitCondition va bloquer
    lockedMutex->unlock(); // dévérouillage du lockedMutex bloqué dans le moniteur
    WaitingLogger::getInstance()->addWaiting(name); // rajout du thread dans la queue d'attente du logger
    bool result = cond.wait(&mutex); // blocage sur la QWaitcondition --> libération du mutex
    WaitingLogger::getInstance()->removeWaiting(name); // une fois libéré par un signal (wakeOne),
                                                                    // le thread se retir lui même de la queue d'attente du logger
    lockedMutex->lock(); // revérouillage du même lockedMutex pour garantir l'atomicité de la transition de l'état waiting à l'état
                         // vérouillé
    mutex.unlock();
    return result;
}

void OWaitCondition::wakeOne(){
    cond.wakeOne();
}

void OWaitCondition::wakeAll(){
    cond.wakeAll();
}
