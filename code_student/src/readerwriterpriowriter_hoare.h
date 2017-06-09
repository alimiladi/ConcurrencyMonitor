#ifndef READERWRITERPRIOWRITER_HOARE_H
#define READERWRITERPRIOWRITER_HOARE_H
#include "abstractreaderwriter.h"
#include "ohoaremonitor.h"

class ReaderWriterPrioWriter_Hoare :
        public AbstractReaderWriter, public OHoareMonitor {
protected:

    Condition attenteRedaction;
    Condition attenteLecture;

    int nbLecteurs;
    bool redactionEnCours;
    int nbRedacteursEnAttente;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioWriter_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        name("Reader-Writer-PrioWriter_Hoare"){}

    void lockReading() {
        monitorIn();
        if ((redactionEnCours) || (nbRedacteursEnAttente > 0)) {
            wait(attenteLecture);
            signal(attenteLecture);
        }
        nbLecteurs ++;
        monitorOut();
    }

    void unlockReading() {
        monitorIn();
        nbLecteurs --;
        if (nbLecteurs == 0) {
            signal(attenteRedaction);
        }
        monitorOut();
    }

    void lockWriting() {
        monitorIn();
        if ((nbLecteurs > 0) || (redactionEnCours)) {
            nbRedacteursEnAttente ++;
            wait(attenteRedaction);
            nbRedacteursEnAttente --;
        }
        redactionEnCours = true;
        monitorOut();
    }

    void unlockWriting() {
        monitorIn();
        redactionEnCours = false;
        if (nbRedacteursEnAttente > 0)
            signal(attenteRedaction);
        else
            signal(attenteLecture);
        monitorOut();
    }
};



#endif // READERWRITERPRIOWRITER_HOARE_H
