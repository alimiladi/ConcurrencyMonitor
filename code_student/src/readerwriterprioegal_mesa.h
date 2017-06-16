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

        while(!libre){
            fifo.wait(&mutex);
        }
        libre = false;

        /*if(ecriture){
            attenteLecture.wait(&mutex);
        }*/

        nbLecture++;


        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecture --;
        libre = true;
        fifo.wakeOne();
        //attenteLecture.wakeOne();
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();

        while(!libre){
            fifo.wait(&mutex);
        }
        libre = false;

        /*if (nbLecture > 0 || ecriture) {
            attenteEcriture.wait(&mutex);
        }*/

        ecriture = true;

        mutex.unlock();

    }

    void unlockWriting() {
        mutex.lock();
        ecriture = false;
        libre = true;
        fifo.wakeOne();
        //attenteEcriture.wakeOne();
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOEGAL_MESA_H
