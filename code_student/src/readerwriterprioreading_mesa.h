#ifndef READERWRITERPRIOREADING_MESA_H
#define READERWRITERPRIOREADING_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"


class ReaderWriterPrioReading_Mesa :
        public AbstractReaderWriter {
protected:
    OWaitCondition attenteLecteurs;
    OWaitCondition attenteTour;
    OMutex mutex;
    int nbLecteurs;
    bool redacteur;
    bool lecteurDansFifo;
    //int nbRedacteursAttente;
    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioReading_Mesa() :
        nbLecteurs(0),
        redacteur(false),
        /*nbRedacteursAttente(0),*/
        lecteurDansFifo(false),
        name("ReaderWriterPrioReading_Mesa")
    {
        attenteLecteurs.setName("attenteLecteurs");
        attenteTour.setName("attenteTour");
        mutex.setName("mutex");
    }

    void lockReading() {
      mutex.lock();
      while(lecteurDansFifo){
          attenteLecteurs.wait(&mutex);
      }
      if(redacteur){
          lecteurDansFifo = true;
          attenteTour.wait(&mutex);
          lecteurDansFifo = false;
      }
      attenteLecteurs.wakeOne();
      nbLecteurs++;
      mutex.unlock();
    }

    void unlockReading() {
      mutex.lock();
      nbLecteurs--;
      if (nbLecteurs==0) {
        attenteTour.wakeOne();
      }
      else{
          attenteLecteurs.wakeOne();
      }
      mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();
        if (nbLecteurs > 0 || redacteur) {
            //nbRedacteursAttente++;
            attenteTour.wait(&mutex);
            //nbRedacteursAttente--;
        }
        redacteur = true;
        mutex.unlock();
    }

    void unlockWriting() {
      mutex.lock();
      redacteur = false;
      attenteTour.wakeOne();
      mutex.unlock();
    }
};

#endif // READERWRITERPRIOREADING_MESA_H
