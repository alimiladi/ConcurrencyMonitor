/** @file owaitcondition.h
 *  @brief wrapper de QWaitCondition pour fournir un service de logs
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug pas de bugs connus
 *
 * Cette classe se base sur les caractéristiques de QWaitCondition.
 * Le but ici est de pouvoir fournir un service de logs sur
 * une condition lorsque les threads lecteurs ou rédacteurs voudront
 * accéder à une ressource.
 */

#ifndef PWAITCONDITION_H
#define PWAITCONDITION_H

#include "omutex.h"
#include <QWaitCondition>

class OWaitCondition
{
public:

    /**
     * @brief OWaitCondition
     * constructeur de la classe
     */
    OWaitCondition();

    /**
     * @brief wait
     * @param lockedMutex: le mutex à déverrouiller
     * @return vrai si peut prendre le mutex et faux sinon
     * méthde wrappée de QWaitcondition pour pouvoir fournir un
     * service de logs
     */
    bool wait(OMutex *lockedMutex);

    /**
     * @brief wakeAll
     * identique à celle de QWaitCondition
     */
    void wakeAll();

    /**
     * @brief wakeOne
     * identique à celle de QWaitCondition
     */
    void wakeOne();

    /**
     * @brief setName
     * @param name: nom de la condition
     * sert à setter le nom de la condition pour les logs
     */
    void setName(QString name);

private:

    QWaitCondition cond;
    QString name;
    QMutex mutex;
};

#endif // PWAITCONDITION_H
