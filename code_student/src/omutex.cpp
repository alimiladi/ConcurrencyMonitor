#include "omutex.h"

OMutex::OMutex() :
    mutex(QMutex::NonRecursive),
    sem(1),
    nb_acces(0){}


void OMutex::setName(QString name)
{
    this->name = name;
}


void OMutex::lock()
{
    mutex.lock();
    // Si personne n'a vérouillé le mutex
    if(nb_acces == 0)
    {
        // Incrément du nombre de threads voulant vérouiller le mutex
        nb_acces ++;
        // Acquisition d'un sémaphore à 1 qui fait office de mutex
        sem.acquire();
        // Libération du mutex de protection car sur que acquire n'est pas bloquant
        mutex.unlock();
    }

    // Si par contre au moins un thread l'a vérouillé
    else if (nb_acces > 0)
    {
        // Incrément du nombre de threads voulant le vérouiller et blocage sur le sémaphore
        nb_acces ++;
        sem.acquire();
        // Rajout du nom du thread appelant dans la liste du mutex
        ReadWriteLogger::getInstance()->addWaiting(name);
        mutex.unlock();
    }
}


void OMutex::unlock(){
    mutex.lock();
    // Si au moins un thread a vérouillé le mutex
    if (nb_acces > 0) {
        // Décrément du compteur
        nb_acces -- ;
        // Libération du sémaphore
        sem.release();
        // Retrait du nom du thread depuis la file d'attente du mutex
        ReadWriteLogger::getInstance()->removeWaiting(name);
    }
    mutex.unlock();
}

bool OMutex::tryLock(){
    mutex.lock();
    // Si c'est le premier thread ==> personne en attente
    if(nb_acces == 0){
        //Il incrémente le compteur
        nb_acces++;
        //Acquiert le sémaphore
        sem.acquire();
        mutex.unlock();
        // Retourne que l'opration s'est bien passée
        return true;
    }
    mutex.unlock();
    //Dans le cas contraire, aucun acquire n'est fait et la valeur false est
    // simplement retournée
    return false;
}
