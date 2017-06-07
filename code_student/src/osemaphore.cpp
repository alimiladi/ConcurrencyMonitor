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

void OSemaphore::setThreadName(QString name){
    this->thread_name = name;
}

/**
 * @brief OSemaphore::acquire
 */
void OSemaphore::acquire(){
    //Incrément du nombre de threads voulants acquérir le sémaphore
    mutex.lock();
    nb_access ++;
    if (nb_access <= initial_capacity) { // si <= n
        mutex.unlock(); // libération du mutex car plus de modif de la variable nb_access
        addResourceAccess(thread_name); // On rajoute le thread dans la liste des threads accédants à la ressource
    }
    else{ // sinon on le rajoute dans la liste des threads en attente sur cette resource
        mutex.unlock(); // libération du mutex
        addWaiting(thread_name, name);
        updateView();
    }
    sem.acquire();
}

void OSemaphore::release(){
    sem.release();
}

bool OSemaphore::tryAcquire(){
    return sem.tryAcquire();
}
