/** @file ohoaremonitor.h
 *  @brief wrapper du hoare monitor pour fournir un service de logs
 *
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug pas de bugs connus
 *
 * Cette classe se base sur hoaremonitor.h. Le but ici est de pouvoir
 * fournir un service de logs sur les conditions du moniteur lorsque
 * les threads lecteurs ou rédacteurs voudront accéder à une ressource.
 * Les commentaire originaux ont été conservés en anglais.
 * Chacune des fonctions ci-dessous mettent à jours les logs en plus
 * de faire le travail décrit en commentaire.
 */

#ifndef OHOAREMONITOR_H
#define OHOAREMONITOR_H

#include "owaitcondition.h"
#include "hoaremonitor.h"


class OHoareMonitor
{
protected:

    /**
     * @brief Classe Condition, sous-classe du moniteur de hoare
     * Cette classe représente la condition sur laquelle les threads
     * vont devoir attendre ou libérer d'autres threads. Celle-ci
     * fournit un service de logs.
     */
    class Condition{
    public:
        Condition();
        HoareMonitor::Condition *getCondition();
        QString getName();
        void setName(QString name);

    private:
        HoareMonitor::Condition condition;
        QString name;
    };

public:
    OHoareMonitor();

    /**
     * This function has to be called at the beginning of each function being
     * an entry point to the monitor.
     */
    void monitorIn();

    /**
     * This function has to be called at the end of each function being
     * an entry point to the monitor.
     */
    void monitorOut();

    /**
     * This function implements the waiting on a condition, as defined by Hoare.
     * When the thread is waken by a signal, it continues with the mutual
     * exclusion.
     */
    void wait(Condition &cond);

    /**
     * This function implements the signaling of a condition, as defined by
     * Hoare. If no thread is waiting for the condition, then nothing happens,
     * but if there is one the thread calling signal is suspended, waiting for
     * the other one to finish.
     */
    void signal(Condition &cond);


private:

    HoareMonitor hoareMonitor;
    QMutex mutex;
    QSemaphore lockedMutex;
    unsigned int nb_waiting;
};

#endif // OHOAREMONITOR_H
