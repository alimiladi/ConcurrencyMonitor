#ifndef READERWRITERPRIOEGAL_HOARE_H
#define READERWRITERPRIOEGAL_HOARE_H

#include "ohoaremonitor.h"
#include "abstractreaderwriter.h"

class ReaderWriterPrioEgal_Hoare : public OHoareMonitor, public AbstractReaderWriter
{
/*protected:
    Condition attenteLecture;
    Condition attenteEcriture;

    int nbLecture;
    int nbEcriture;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioEgal_Hoare() :
        nbLecture(0),
        nbEcriture(0),
        name("Reader-Writer-PrioEgal_Hoare")
    {
        attenteLecture.setName("attenteLecture");
        attenteEcriture.setName("attenteEcriture");
    }

    void lockReading() {
        monitorIn();
        if (nbEcriture > 0) {
            wait(attenteLecture);
            signal(attenteLecture);
        }
        nbLecture ++;
        monitorOut();
    }

    void unlockReading() {
        monitorIn();
        nbLecture --;
        if (nbLecture == 0)
            //signal(attenteLecture);
            signal(attenteEcriture);
        monitorOut();
    }

    void lockWriting() {
        monitorIn();
        if (nbLecture > 0 || nbEcriture > 0) {
            wait(attenteEcriture);
        }
        nbEcriture ++;
        monitorOut();
    }

    void unlockWriting() {
        monitorIn();
        nbEcriture --;
        if (nbEcriture == 0)
           // signal(attenteEcriture);
            signal(attenteLecture);
        monitorOut();
    }*/




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

    ReaderWriterPrioEgal_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        name("Reader-Writer-PrioEgal_Hoare"){
        attenteLecture.setName("attenteLecture");
        attenteRedaction.setName("attenteRedaction");
    }

    void lockReading() {
        monitorIn();
        if (redactionEnCours) {
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
        signal(attenteLecture);
        monitorOut();
    }





};

#endif // READERWRITERPRIOEGAL_HOARE_H
