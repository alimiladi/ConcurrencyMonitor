#ifndef READERWRITERPRIOREADING_HOARE_H
#define READERWRITERPRIOREADING_HOARE_H

class ReaderWriterPrioReading_Hoare : public OHoareMonitor, public AbstractReaderWriter
{
protected:

    Condition attenteRedaction;
    Condition attenteLecture;

    bool libre;
    int nbLecteurs;
    bool redactionEnCours;
    int nbRedacteursEnAttente;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioReading_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        libre(true),
        name("ReaderWriterPrioReading_Hoare"){
        attenteLecture.setName("attenteLecture");
        attenteRedaction.setName("attenteRedaction");
    }

    void lockReading() {
        monitorIn();

        monitorOut();
    }

    void unlockReading() {
        monitorIn();

        monitorOut();
    }

    void lockWriting() {
        monitorIn();

        monitorOut();
    }

    void unlockWriting() {
        monitorIn();

        monitorOut();
    }
};

#endif // READERWRITERPRIOREADING_HOARE_H
