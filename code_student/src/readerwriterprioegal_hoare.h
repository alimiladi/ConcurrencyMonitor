#ifndef READERWRITERPRIOEGAL_HOARE_H
#define READERWRITERPRIOEGAL_HOARE_H

#include "ohoaremonitor.h"
#include "abstractreaderwriter.h"

class ReaderWriterPrioEgal_Hoare : public OHoareMonitor, public AbstractReaderWriter
{

protected:

    Condition attenteRedaction;
    Condition attenteLecture;
    Condition fifo;

    bool libre;
    int nbLecteurs;
    bool redactionEnCours;
    int nbRedacteursEnAttente;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioEgal_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        libre(true),
        name("Reader-Writer-PrioEgal_Hoare"){
        attenteLecture.setName("attenteLecture");
        attenteRedaction.setName("attenteRedaction");
        fifo.setName("fifo");
    }

    void lockReading() {             
        monitorIn();

        if(!libre){
            wait(fifo);
        }
        libre = false;

        if (redactionEnCours) {
            wait(attenteLecture);
            signal(attenteLecture);
        }
        nbLecteurs ++;

        libre = true;
        signal(fifo);

        monitorOut();
    }

    void unlockReading() {
        monitorIn();
        nbLecteurs --;
        if (nbLecteurs == 0) {
            //signal(attenteLecture);
            signal(attenteRedaction);
        }

        libre = true;
        signal(fifo);

        monitorOut();
    }

    void lockWriting() {
        monitorIn();

        if(!libre){
            wait(fifo);
        }
        libre = false;

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
        //signal(attenteRedaction);
        signal(attenteLecture);

        libre = true;
        signal(fifo);

        monitorOut();
    }





};

#endif // READERWRITERPRIOEGAL_HOARE_H
