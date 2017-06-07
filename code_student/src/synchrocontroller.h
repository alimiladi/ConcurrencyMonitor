#ifndef SYNCHROCONTROLLER_H
#define SYNCHROCONTROLLER_H
#include <QSemaphore>


class SynchroController
{
    //pour mettre le main en attente
    //un thread se mettant en pause va débloquer le main
    QSemaphore *mainWaiting;

    SynchroController();

public:

    static SynchroController *getInstance();

    void pause();

    void resume();

    QSemaphore * getMainWaiting();

};

#endif // SYNCHROCONTROLLER_H
