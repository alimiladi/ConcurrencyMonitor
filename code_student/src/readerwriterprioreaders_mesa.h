#ifndef READERWRITERPRIOREADERS_MESA_H
#define READERWRITERPRIOREADERS_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"


class ReaderWriterPrioReaders_Mesa :
        public AbstractReaderWriter {
protected:

    OWaitCondition attenteEcriture;
    OWaitCondition attenteLecture;
    OMutex mutex;

    int nbLecteurs;
    int nbLecteursEnAttente;
    int nbRedacteursEnAttente;
    bool redactionEnCours;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioReaders_Mesa() :
        nbLecteurs(0),
        nbLecteursEnAttente(0),
        nbRedacteursEnAttente(0),
        redactionEnCours(false),
        name("Reader-Writer-PrioReaders_Mesa")
    {
        attenteEcriture.setName("attenteEcriture");
        attenteLecture.setName("attenteLecture");
        mutex.setName("mutex");
    }

    void lockReading() {
        mutex.lock();
        if (redactionEnCours) { //Attente sur la condition seuelement si un lecteur est déja dans la ressource
            nbLecteursEnAttente ++;
            attenteLecture.wait(&mutex);
        }
        nbLecteurs ++; //Incrémentation du nombre de lecteurs de toute façon car soit pas de rédaction en cours
        // soit réception d'un wakeOne ==> déblocage du wait
        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecteurs --;
        if (nbLecteurs == 0 && !nbLecteursEnAttente) { //Réveil d'un rédacteur seulement si c'est le dernier des lecteurs
            attenteEcriture.wakeOne();
        }
        else
            attenteLecture.wakeOne(); //Si encore des lecteurs en attente lors de la sortie de la resource, en libérer un
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();
        if ( nbLecteurs || nbLecteursEnAttente || redactionEnCours ) {
            //Attente sur la condition dans le cas ou au moins un lecteur est en cours de lecture ou en attente de lecture
            // ou qu'un autre rédacteur est en train d'écrire.
            nbRedacteursEnAttente ++;
            attenteEcriture.wait(&mutex);
        }
        else
            redactionEnCours = true;
        mutex.unlock();
    }

    void unlockWriting() {
        mutex.lock();
        redactionEnCours = false;
        if (nbLecteursEnAttente) { //Libération d'un lecteur si au moins un en attente
            nbLecteursEnAttente --;
            nbLecteurs++;
            attenteLecture.wakeOne();
            mutex.unlock();
        }
        else if (nbRedacteursEnAttente) {//Si pas de lecteurs en attente mais rédacteurs en attente, en libérer un
            nbRedacteursEnAttente --;
            redactionEnCours = true;
            attenteEcriture.wakeOne();
            mutex.unlock();
        }
    }
};

#endif // READERWRITERPRIOREADERS_MESA_H
