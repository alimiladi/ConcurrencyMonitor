#ifndef READERWRITERPRIOEGAL_MESA_H
#define READERWRITERPRIOEGAL_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"

class ReaderWriterPrioEgal_Mesa : public AbstractReaderWriter{

protected:
    OMutex mutex;
    OWaitCondition attenteLecture;
    int nbAttenteLecture;
    OWaitCondition attenteEcriture;
    int nbAttenteEcriture;
    int nbLecture;
    int nbEcriture;
    QString name;



public:

    QString getName(){
        return name;
    }


    ReaderWriterPrioEgal_Mesa() :
        nbAttenteLecture(0),
        nbAttenteEcriture(0),
        nbLecture(0),
        nbEcriture(0), name("Reader-Writer-PrioEgal_Mesa")
    {
        mutex.setName("mutex");
        attenteLecture.setName("attenteLecture");
        attenteEcriture.setName("attenteEcriture");
    }

    void lockReading() {
        mutex.lock();
        if (nbEcriture > 0) {
            nbAttenteLecture ++;
            attenteLecture.wait(&mutex);
        }
        else
            nbLecture ++;
        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecture --;
        if (nbLecture == 0) {
            attenteEcriture.wakeAll();
            nbEcriture = nbAttenteEcriture;
            nbAttenteEcriture = 0;
        }
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();
        if (nbLecture > 0 || nbEcriture > 0) {
            nbAttenteEcriture ++;
            attenteEcriture.wait(&mutex);
        }
        else
            nbEcriture ++;
        mutex.unlock();

    }

    void unlockWriting() {
        mutex.lock();
        nbEcriture --;
        if (nbEcriture == 0) {
            attenteLecture.wakeAll();
            nbLecture = nbAttenteLecture;
            nbAttenteLecture = 0;
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOEGAL_MESA_H
