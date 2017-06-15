#include "omutex.h"

OMutex::OMutex() : mutex(QMutex::NonRecursive), sem(1), nb_acces(0)
{

}

void OMutex::setName(QString name){
    this->name = name;
}



void OMutex::lock(){
    mutex.lock(); // Protection par un mutex
    if(nb_acces == 0){ // Si personne n'a vérouillé le mutex
        nb_acces ++; // Incrément du nombre de threads voulant vérouiller le mutex
        sem.acquire(); // Acquisition d'un sémaphore à 1 qui fait office de mutex
        mutex.unlock(); // Libération du mutex de protection car sur que acquire n'est pas bloquant
    }
    else if (nb_acces > 0){ // Si par contre au moins un thread l'a vérouillé
        nb_acces ++;
        sem.acquire(); // Incrément du nombre de threads voulant le vérouiller et blocage sur le sémaphore
        ReadWriteLogger::getInstance()->addWaiting(name); // Rajout dans la file d'attente du logger
        mutex.unlock();
    }
}

void OMutex::unlock(){
    mutex.lock();
    if (nb_acces > 0) { // Si au moins un thread a vérouillé le mutex
        nb_acces -- ; // Décrément du compteur
        sem.release(); // Libération du sémaphore
        ReadWriteLogger::getInstance()->removeWaiting(name); // Retrait de la file d'attented du logger
    }
    mutex.unlock();
}

bool OMutex::tryLock(){
    mutex.lock();
    if(nb_acces == 0){
        nb_acces++;
        sem.acquire();
        mutex.unlock();
        return true;
    }
    mutex.unlock();
    return false;
}
