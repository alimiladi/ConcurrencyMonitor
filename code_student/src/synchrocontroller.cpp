#include "synchrocontroller.h"


SynchroController::SynchroController()
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

    //On redonne la main au main thread pour demander à l'utilisateur la suite
    mainWaiting->release();

    //On met le thread en pause
    threadWaiting->acquire();
}

void SynchroController::resume()
{

    //On libère le thread
    threadWaiting->release();
}

QSemaphore* SynchroController::getMainWaiting()
{
    return mainWaiting;
}
