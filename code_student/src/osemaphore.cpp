#include "osemaphore.h"

/**
 * @brief OSemaphore::OSemaphore
 * Constructeur
 * @param n
 */
OSemaphore::OSemaphore(int n):
    sem(n),
    initial_capacity(n),
    nb_access(0),
    mutex(QMutex::NonRecursive){}

/**
 * @brief OSemaphore::setName
 * Définit le nom du sémaphore
 * @param name
 */
void OSemaphore::setName(QString name){
    this->name = name;
}

/*void OSemaphore::setThreadName(QString thread_name){
    this->thread_name = thread_name;
}*/

/**
 * @brief OSemaphore::acquire
 */
void OSemaphore::acquire(unsigned int id){
    //Incrément du nombre de threads voulants acquérir le sémaphore
    mutex.lock();
    nb_access ++;
    if (nb_access > initial_capacity) {
        // si le nombre de threads appelant acquire est > n on le rajoute dans la liste d'attente
        ReadWriteLogger::getInstance()->addWaiting(name, id);
    }
    mutex.unlock(); // libération du mutex
    sem.acquire();
}

void OSemaphore::release(unsigned int id){
    mutex.lock();
    if(nb_access > initial_capacity){
        ReadWriteLogger::getInstance()->removeWaiting(name, id);
    }
    nb_access --;
    sem.release();
    mutex.unlock();
}

bool OSemaphore::tryAcquire(){
    mutex.lock();
    nb_access ++;
    if(nb_access > initial_capacity){
        nb_access--;
        mutex.unlock();
        return false;
    }
    sem.acquire();
    mutex.unlock();
    return true;
}
