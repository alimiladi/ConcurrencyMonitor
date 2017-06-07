#include "synchrocontroller.h"

SynchroController::SynchroController()
{
       mainWaiting = new QSemaphore(0);
}


SynchroController *SynchroController::getInstance()
{
    static SynchroController *instance = new SynchroController();
    return instance;
}

void SynchroController::pause()
{
    //on redonne la main au main thread pour demander à l'utilisateur la suite
    mainWaiting->release();

    //on met le thread en pause

}

void SynchroController::resume()
{

    //on libère le thread
}

QSemaphore* SynchroController::getMainWaiting(){
    return mainWaiting;
}
