#include "osemaphore.h"

OSemaphore::OSemaphore(int n):
    sem(n),
    initial_capacity(n),
    nb_access(0),
    mutex(QMutex::NonRecursive){}


void OSemaphore::setName(QString name)
{
    this->name = name;
}


void OSemaphore::acquire()
{
    mutex.lock();
    //Incrément du nombre de threads voulants acquérir le sémaphore
    nb_access ++;
    if (nb_access > initial_capacity) {
        //Si le nombre de threads appelant acquire est > n on le rajoute dans
        // la liste d'attente
        ReadWriteLogger::getInstance()->addWaiting(name);
    }
    mutex.unlock(); //Libération du mutex
    sem.acquire();
}

void OSemaphore::release()
{
    mutex.lock();
    //Si il existe des threads bloqués
    if(nb_access > initial_capacity){
        //On les enlève de la liste d'attente du sémaphore
        ReadWriteLogger::getInstance()->removeWaiting(name);
    }
    //Décrément du nombre disponible (available dans QSemaphore) du sémaohore de
    // toute façon
    nb_access --;
    // Libération du sémaphore
    sem.release();
    mutex.unlock();
}

bool OSemaphore::tryAcquire(){
    mutex.lock();
    //Incrément du nombre de threads voulant acquérir le sémaphore
    nb_access ++;
    //Si le acquire est bloquqant (on l'appelle plus de fois que la capacité du
    // sémaphore) alors, nous n'allons rien faire et retourner false
    if(nb_access > initial_capacity){
        nb_access--;
        mutex.unlock();
        return false;
    }
    //Sinon on acquire le sémaphore et on retourne true
    sem.acquire();
    mutex.unlock();
    return true;
}
