#include "synchrocontroller.h"
#include <iostream>
#include "waitinglogger.h"
#include <QThread>
#include "reader_writer_thread.h"


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

void SynchroController::pause()
{
    fifo->acquire();
    mutex.lock();

    //bool firstTime = ((ReaderWriterThread*)ReaderWriterThread::currentThread())->getFirstTime();

    //si c'est la première fois que le thread se met en pause, il n y a pas de logs à afficher
    //if(firstTime){
       // std::cout << "Le " << qPrintable(QThread::currentThread()->objectName()) << " va demander pour la premire fois l acces a la ressource." << std::endl;
    //}else{
        //on affiche les logs qui ont été fait depuis la dernière pause
        WaitingLogger::getInstance()->printLogs();
    //}

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
