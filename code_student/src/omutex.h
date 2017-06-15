/** @file omutex.h
 *  @brief wrapper du QMutex pour fournir un service de logs
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug pas de bugs connus
 *
 * Cette classe se base sur les caractéristiques du QMutex.
 * Le but ici est de pouvoir fournir un service de logs sur
 * un mutex lorsque les threads lecteurs ou rédacteurs voudront
 * accéder à une ressource.
 */

#ifndef PMUTEX_H
#define PMUTEX_H

#include <QSemaphore>
#include <QMutex>
#include "waitinglogger.h"


class OMutex
{
public:
    /**
     * @brief OMutex
     * Constructeur de la classe
     */
    OMutex();

    /**
     * @brief lock
     * sert à verrouiller le mutex et mettre à jour les logs
     */
    void lock();

    /**
     * @brief tryLock
     * sert à tester la disponibilité du mutex
     * @return vrai si disponible (verrouille le mutex) et faux sinon
     */
    bool tryLock();

    /**
     * @brief unlock
     * sert à déverrouiller le mutex et mettre à jour les logs
     */
    void unlock();

    /**
     * @brief setName
     * @param name: nom du mutex
     * sert à nommer le mutex
     */
    void setName(QString name);


private:
    QMutex mutex;
    QSemaphore sem;
    unsigned int nb_acces;
    QString name;

};

#endif // PMUTEX_H
