#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H
#include <QSemaphore>
#include <QMutex>
#include <QVector>


class SynchroController
{
private:
    //pour mettre le main en attente
    //un thread se mettant en pause va débloquer le main
    QSemaphore *mainWaiting;

    //file d'attente pour les threads
    QSemaphore *threadWaiting;

    QSemaphore *fifo;

    // nombre de threads bloqués en attente
    unsigned int nbPaused = 0;

    QMutex mutex;

    SynchroController();
public:

    static SynchroController *getInstance();

    void pause();

    void resume();

    //getters
    QSemaphore * getMainWaiting();

};

#endif // SYNCHROCONTROLLER_H
