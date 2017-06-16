/** @file readerwriterprioreading_hoare.h
 *  @brief ressource accessible avec moniteur de hoare
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe représente une ressource que des lecteurs et rédateurs
 * vondront accéder. Pour y accéder ou en sortir, les threads devront
 * appeler une des méthodes ci-dessous.
 * La ressource hérite pour cela d'une ressource abstraite et du moniteur
 * de hoare fournissant un service de logs.
 * Le but ici est de gérer une prorité pour les lecteurs en lecture.
 */


#ifndef READERWRITERPRIOREADING_HOARE_H
#define READERWRITERPRIOREADING_HOARE_H

class ReaderWriterPrioReading_Hoare : public OHoareMonitor, public AbstractReaderWriter
{
protected:

    //un seul lecteur pourra attendre sur cette file avec les autres
    //rédacteurs, c'est la file principale.
    Condition fifo;
    //les autres lecteurs doivent attendre sur cette file
    Condition attenteLecture;

    int nbLecteurs;
    bool lecteurDansFifo;
    bool redacteur;

    QString name;

public:

    /**
     * @brief getName
     * @return le nom de la ressource
     */
    QString getName(){
        return name;
    }

    /**
     * @brief ReaderWriterPrioReading_Hoare
     * constructeur de la ressource
     */
    ReaderWriterPrioReading_Hoare() :
        nbLecteurs(0),
        lecteurDansFifo(false),
        redacteur(false),
        name("ReaderWriterPrioReading_Hoare"){
        attenteLecture.setName("attenteLecture");
        fifo.setName("fifo");
    }

    /**
     * @brief lockReading
     * pour accéder à la ressource en lecture
     */
    void lockReading() {
        monitorIn();

        //si déjà un lecteur attend parmis les rédacteurs alors
        //les autres lecteurs attendront sur la file des lecteurs
        while(lecteurDansFifo){
            wait(attenteLecture);
        }

        //le lecteur vérifie s'il n'y a pas de rédaction en cours sinon il attend
        //dans le fifo
        if(redacteur){
            lecteurDansFifo = true;
            wait(fifo);
            lecteurDansFifo = false;
        }

        //on donne la priorité aux autres lecteurs car il y a lecture
        signal(attenteLecture);
        nbLecteurs++;

        monitorOut();
    }

    /**
     * @brief unlockReading
     * pour sortir de la ressource en lecture
     */
    void unlockReading() {
        monitorIn();

        nbLecteurs--;

        //le dernier lecteur réveille un thread du fifo
        if(nbLecteurs == 0){
            signal(fifo);
        }
        //sinon on donne la priorité au lecteurs qui attendent
        else{
            signal(attenteLecture);
        }

        monitorOut();
    }

    /**
     * @brief lockWriting
     * pour accéder à la ressource en écriture
     */
    void lockWriting() {
        monitorIn();

        //si la ressource est occupée, on attend su le fifo
        if(nbLecteurs > 0 || redacteur){
            wait(fifo);
        }

        redacteur = true;

        monitorOut();
    }

    /**
     * @brief unlockWriting
     * pour sortir de la ressource en écriture
     */
    void unlockWriting() {
        monitorIn();

        redacteur = false;

        //on donne le tour au suivant dans le fifo
        signal(fifo);

        monitorOut();
    }
};

#endif // READERWRITERPRIOREADING_HOARE_H
