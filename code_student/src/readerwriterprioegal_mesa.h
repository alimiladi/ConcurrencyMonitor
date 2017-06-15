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

    OWaitCondition fifo;
    int libre;
    int nbAttenteFifo;



public:

    QString getName(){
        return name;
    }


    ReaderWriterPrioEgal_Mesa() :
        nbAttenteLecture(0),
        nbAttenteEcriture(0),
        nbLecture(0),
        nbEcriture(0),
        name("Reader-Writer-PrioEgal_Mesa"),
        libre(true),
        nbAttenteFifo(0)
    {
        mutex.setName("mutex");
        attenteLecture.setName("attenteLecture");
        attenteEcriture.setName("attenteEcriture");
        fifo.setName("fifo");
    }

    void lockReading() {
        mutex.lock();

        if(!libre || nbAttenteFifo){
            nbAttenteFifo++;
            fifo.wait(&mutex);
            nbAttenteFifo--;
        }
        libre = false;

        if (nbEcriture > 0) {
            nbAttenteLecture ++;
            attenteLecture.wait(&mutex);
        }
        else{
            nbLecture ++;

        }
        libre = true;
        fifo.wakeOne();
        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecture --;
        if (nbLecture == 0) {
            attenteEcriture.wakeOne();
            //nbAttenteEcriture--;
            //nbEcriture++;
        }
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();

        if(!libre || nbAttenteFifo){
            nbAttenteFifo++;
            fifo.wait(&mutex);
            nbAttenteFifo--;
        }
        libre = false;

        if (nbLecture > 0 || nbEcriture > 0) {
            nbAttenteEcriture ++;
            attenteEcriture.wait(&mutex);
        }
        nbAttenteEcriture--;
        nbEcriture++;

        libre = true;
        fifo.wakeOne();

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
