#ifndef READERWRITERPRIOWRITER_MESA_H
#define READERWRITERPRIOWRITER_MESA_H


#include <QMutex>
#include <QSemaphore>
#include "owaitcondition.h"

#include "abstractreaderwriter.h"
#include "hoaremonitor.h"


class ReaderWriterPrioWriter_Mesa :
        public AbstractReaderWriter {
protected:

    OWaitCondition attenteEcriture;
    OWaitCondition attenteLecture;
    OMutex mutex;

    int nbLecteurs;
    int nbLecteursEnAttente;
    int nbRedacteursEnAttente;
    bool redactionEnCours;

public:
    ReaderWriterPrioWriter_Mesa() :
        nbLecteurs(0),
        nbLecteursEnAttente(0),
        nbRedacteursEnAttente(0),
        redactionEnCours(false)
    {
        attenteEcriture.setName("attenteEcriture");
        attenteLecture.setName("attenteLecture");
        mutex.setName("mutex");
    }

    void lockReading() {
        mutex.lock();
        if ((nbRedacteursEnAttente > 0) || (redactionEnCours)) {
            nbLecteursEnAttente ++;
            attenteLecture.wait(&mutex);
        }
        else
            nbLecteurs ++;
        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecteurs --;
        if (nbLecteurs == 0) {
            attenteEcriture.wakeOne();
        }
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();
        if ((nbLecteurs > 0) || (redactionEnCours)) {
            nbRedacteursEnAttente ++;
            attenteEcriture.wait(&mutex);
        }
        else
            redactionEnCours = true;
        mutex.unlock();
    }

    void unlockWriting() {
        mutex.lock();
        redactionEnCours = false;
        if (nbRedacteursEnAttente) {
            nbRedacteursEnAttente --;
            redactionEnCours = true;
            attenteEcriture.wakeOne();
        }
        else {
            for(int i=0;i<nbLecteursEnAttente; i++) {
                attenteLecture.wakeOne();
            }
            nbLecteurs = nbLecteursEnAttente;
            nbLecteursEnAttente = 0;

        }
        mutex.unlock();
    }
};


#endif // READERWRITERPRIOWRITER_MESA_H
