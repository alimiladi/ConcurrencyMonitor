#include "owaitcondition.h"

OWaitCondition::OWaitCondition() :
    cond(),
    mutex(QMutex::NonRecursive) {}

void OWaitCondition::setName(QString name)
{
    this->name = name;
}

bool OWaitCondition::wait(OMutex *lockedMutex)
{
    //Mutex sur lequel la QWaitCondition va bloquer
    mutex.lock();
    //Dévérouillage du lockedMutex bloqué dans le moniteur
    lockedMutex->unlock();
    //Rajout du nom du thread dans la queue d'attente de la variable condition
    WaitingLogger::getInstance()->addWaiting(name);
    //Blocage sur la QWaitcondition --> libération du mutex
    bool result = cond.wait(&mutex);
    //Une fois libéré par un signal (wakeOne),le thread se retire lui même
    // de la queue d'attente de la variable condition
    WaitingLogger::getInstance()->removeWaiting(name);
    //Revérouillage du même lockedMutex pour garantir l'atomicité de la transition
    // de l'état waiting à l'état vérouillé
    lockedMutex->lock();
    mutex.unlock();
    return result;
}


void OWaitCondition::wakeOne()
{
    //Simple utilisation de la méthode wakeOne de la classe HoareMonitor::Condition
    cond.wakeOne();
}

void OWaitCondition::wakeAll()
{
    //Simple utilisation de la méthode wakeAll de la classe HoareMonitor::Condition
    cond.wakeAll();
}
