#ifndef READERWRITERPRIOREADING_MESA_H
#define READERWRITERPRIOREADING_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"


class ReaderWriterPrioReading_Mesa :
        public AbstractReaderWriter {
protected:
    OWaitCondition attenteLecteurs;
    OWaitCondition attenteRedacteurs;
    OMutex mutex;
    int nbLecteurs;
    bool redacteur;
    int nbRedacteursAttente;
    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioReading_Mesa() :
        nbLecteurs(0),
        redacteur(false),
        nbRedacteursAttente(0),
        name("ReaderWriterPrioReading_Mesa")
    {
        attenteLecteurs.setName("attenteLecteurs");
        attenteRedacteurs.setName("attenteRedacteurs");
        mutex.setName("mutex");
    }

    void lockReading() {
      mutex.lock();
      if(redacteur){
          attenteLecteurs.wait(&mutex);
      }
      nbLecteurs++;
      mutex.unlock();
    }

    void unlockReading() {
      mutex.lock();
      nbLecteurs--;
      if (nbLecteurs==0) {
        attenteRedacteurs.wakeOne();
      }
      else{
          attenteLecteurs.wakeOne();
      }
      mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();
        if (nbLecteurs > 0 || redacteur) {
            nbRedacteursAttente++;
            attenteRedacteurs.wait(&mutex);
            nbRedacteursAttente--;
        }
        redacteur = true;
        mutex.unlock();
    }

    void unlockWriting() {
      mutex.lock();
      redacteur = false;
      mutex.unlock();
    }
};

#endif // READERWRITERPRIOREADING_MESA_H
