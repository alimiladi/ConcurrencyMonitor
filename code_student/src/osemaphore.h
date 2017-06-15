/** @file osemaphore.h
 *  @brief wrapper du QSemaphore pour fournir un service de logs
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug pas de bugs connus
 *
 * Cette classe se base sur les caractéristiques du QSemaphore.
 * Le but ici est de pouvoir fournir un service de logs sur
 * une sémaphore lorsque les threads lecteurs ou rédacteurs voudront
 * accéder à une ressource.
 */

#ifndef PSEMAPHORE_H
#define PSEMAPHORE_H

#include <QSemaphore>
#include "waitinglogger.h"
#include "QMutex"


class OSemaphore
{
public:

    /**
     * @brief OSemaphore
     * @param n: valeur de la sémaphore
     * constructeur de la classe
     */
    OSemaphore(int n);

    /**
     * @brief acquire
     * fournit un service de logs par dessu la méthode acquire de QSemaphore
     */
    void acquire();

    /**
     * @brief release
     * fournit un service de logs par dessu la méthode release de QSemaphore
     */
    void release();

    /**
     * @brief tryAcquire
     * @return vrai si peut acquérir la sémaphore et faux sinon
     * même implémentation que le tryAcquire() de QSemaphore
     */
    bool tryAcquire();

    /**
     * @brief setName
     * @param name: nom de la sémaphore
     * sert à setter un nom pour la sémaphore
     */
    void setName(QString name);


private:
    QSemaphore sem;
    QString name;
    unsigned int nb_access, //nombre de threads ayant essayé d'acquérir le sémaphore
    initial_capacity; // capacité avec laquelle le sémaphore est initialisé
    QMutex mutex; // utilisé pour protéger les accès à la variable nb_access

};

#endif // PSEMAPHORE_H
