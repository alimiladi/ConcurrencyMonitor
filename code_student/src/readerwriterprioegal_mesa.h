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
    bool ecriture;
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
        ecriture(false),
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
        if(ecriture){
            fifo.wait(&mutex);
        }
        if(ecriture){
            attenteLecture.wait()
        }

        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecture --;
        if (nbLecture == 0) {
            attenteEcriture.wakeOne();
        }
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();

        while(!libre){
            fifo.wait(&mutex);
        }
        libre = false;

        if (nbLecture > 0 || ecriture) {
            nbAttenteEcriture ++;
            attenteEcriture.wait(&mutex);
        }
        nbAttenteEcriture--;
        ecriture = true;

        libre = true;
        fifo.wakeOne();

        mutex.unlock();

    }

    void unlockWriting() {
        mutex.lock();
        ecriture = false
        if () {
            attenteLecture.wakeAll();
            nbLecture = nbAttenteLecture;
            nbAttenteLecture = 0;
        }
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOEGAL_MESA_H
