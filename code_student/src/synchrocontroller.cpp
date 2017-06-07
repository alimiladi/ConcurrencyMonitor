#include "synchrocontroller.h"
#include <iostream>


SynchroController::SynchroController():
mutex(QMutex::NonRecursive)
{
       mainWaiting = new QSemaphore(0);
       threadWaiting = new QSemaphore(0);
}


SynchroController *SynchroController::getInstance()
{
    static SynchroController *instance = new SynchroController();
    return instance;
}

void SynchroController::pause()
{
    mutex.lock();
    mutex.unlock();
    //on redonne la main au main thread pour demander à l'utilisateur la suite
    mainWaiting->release();
    //on met le thread en pause
    threadWaiting->acquire();
}

void SynchroController::resume()
{
    //on libère le thread
    threadWaiting->release();
}

QSemaphore* SynchroController::getMainWaiting(){
    return mainWaiting;
}

/*QSemaphore* SynchroController::getThreadWaiting(){
    return threadWaiting;
}*/
