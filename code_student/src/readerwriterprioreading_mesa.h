#ifndef READERWRITERPRIOREADING_MESA_H
#define READERWRITERPRIOREADING_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"


class ReaderWriterPrioReading_Mesa :
        public AbstractReaderWriter {
protected:
    OWaitCondition attenteLecture;
    OWaitCondition attente;
    OMutex mutex;

    int nbLecteurs;
    int nbLecteursEnAttente;
    int nbEnAttente;
    // int nbRedacteursEnAttente;
    bool redactionEnCours;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioReading_Mesa() :
        nbLecteurs(0),
        nbLecteursEnAttente(0),
        nbEnAttente(0),
        redactionEnCours(false),
        name("ReaderWriterPrioReading_Mesa")
    {
        attente.setName("attente");
        attenteLecture.setName("attenteLecture");
        mutex.setName("mutex");
    }

    void lockReading() {
        mutex.lock();
        nbLecteurs ++;
        if(nbLecteurs == 1) {
            nbEnAttente ++;
            attente.wait(&mutex);
            nbEnAttente --;
        }
        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecteurs --;
        if(nbLecteurs == 0) {
            attente.wakeOne();
        }
        mutex.unlock();
    }

    void lockWriting() {

    }

    void unlockWriting() {

    }
};

#endif // READERWRITERPRIOREADING_MESA_H
