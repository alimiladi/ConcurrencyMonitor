#include "synchrocontroller.h"
#include <iostream>


SynchroController::SynchroController():
mutex(QMutex::NonRecursive)
{
       mainWaiting = new QSemaphore(0);
       threadWaiting = new QSemaphore(0);
       fifo = new QSemaphore(1);
}


SynchroController *SynchroController::getInstance()
{
    static SynchroController *instance = new SynchroController();
    return instance;
}

void SynchroController::pause(unsigned int id, bool isReader, bool isLocking)
{
    fifo->acquire();
    mutex.lock();

//    nbPaused ++;
//    if(isLocking){
//        if(isReader){
//            std::cout << "le lecteur " << id  << " veut acceder a la ressource" << std::endl;
//        }else {
//            std::cout << "le redacteur " << id  << " veut acceder a la ressource" << std::endl;
//        }
//    }else{
//        if(isReader){
//           std::cout << "le lecteur " << id  << " veut quitter la ressource" << std::endl;
//        }else {
//            std::cout << "le redacteur " << id  << " veut quitter la ressource" << std::endl;
//        }
//    }
//    if(nbPaused > 0)

    //on redonne la main au main thread pour demander à l'utilisateur la suite
    mainWaiting->release();
    mutex.unlock();
    //on met le thread en pause
    threadWaiting->acquire();
}

void SynchroController::resume()
{
    fifo->release();
    //on libère le thread
    threadWaiting->release();
}

QSemaphore* SynchroController::getMainWaiting(){
    return mainWaiting;
}

/*QSemaphore* SynchroController::getThreadWaiting(){
    return threadWaiting;
}*/
