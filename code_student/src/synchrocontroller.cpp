#include "synchrocontroller.h"
#include <iostream>
#include "waitinglogger.h"
#include <QThread>
#include "reader_writer_thread.h"


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
