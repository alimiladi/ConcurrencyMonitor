#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H
#include <QSemaphore>


class SynchroController
{
    //pour mettre le main en attente
    //un thread se mettant en pause va débloquer le main
    QSemaphore *mainWaiting;

    //file d'attente pour les threads
    QSemaphore *threadWaiting;

    SynchroController();

public:

    static SynchroController *getInstance();

    void pause();

    void resume();

    //getters
    QSemaphore * getMainWaiting();
    //QSemaphore * getThreadWaiting();

};

#endif // SYNCHROCONTROLLER_H
