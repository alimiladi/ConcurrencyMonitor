/** @file synchrocontroller.h
 *  @brief synchronise l'application en marquant des pauses
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe fournit les méthodes nécessaires à marquer des pauses
 * et des départs dans l'application puisque chaque thread
 * (lecteur ou rédacteurs), devra marquer une pause entre les différents
 * accès/sortie de la ressource.
 * Le travail consiste à passer la main au programme principal quand il faut
 * attendre une saisie pour continuer. Sinon le programme pincipal doit être
 * mis en attente pendant qu'un lecteur ou rédacteur effectuera ses accès/sorties.
 * Le but de tout ça est d'avoir le temps de visualiser les logs pour une étape
 * donnée avant de passer à la suite.
 */

#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H
#include <QSemaphore>
#include <QMutex>
#include <QVector>


/**
 * @brief The SynchroController class
 * est un singleton
 */
class SynchroController
{

private:

    //pour mettre le main en attente
    //un thread se mettant en pause va débloquer le main
    QSemaphore *mainWaiting;

    //file d'attente pour les threads
    QSemaphore *threadWaiting;




    /**
     * @brief SynchroController
     * constructeur de la classe
     */
    SynchroController();

public:

    /**
     * @brief getInstance
     * @return l'instance unique du Synchro Controller
     */
    static SynchroController *getInstance();

    /**
     * @brief pause
     * pour mettre un thread lecteur ou rédacteur en pause
     */
    void pause();

    /**
     * @brief resume
     * pour libérer un thread en pause
     */
    void resume();

    /**
     * @brief getMainWaiting
     * @return la semaphore sur laquelle attend le thread principal
     */
    QSemaphore * getMainWaiting();

};

#endif // SYNCHROCONTROLLER_H
