#ifndef PSEMAPHORE_H
#define PSEMAPHORE_H

#include <QSemaphore>
#include "waitinglogger.h"
#include "QMutex"

/**
 * @brief The OSemaphore class
 * Sémaphore adapté aux besoins de l'appli, hérite de ReadWriteLogger pour pouvoir faire
 * des logs s'il y'a des threads qui bloquent dessus.
 *
 */
class OSemaphore
{
public:
    //! Constructeur pareil à celui de QSemaphore
    OSemaphore(int n);

    void acquire();

    void release();

    bool tryAcquire();

    void setName(QString name);


private:
    QSemaphore sem;
    QString name;
    unsigned int nb_access, //nombre de threads ayant essayé d'acquérir le sémaphore
    initial_capacity; // capacité avec laquelle le sémaphore est initialisé
    QMutex mutex; // utilisé pour protéger les accès à la variable nb_access

};

#endif // PSEMAPHORE_H
