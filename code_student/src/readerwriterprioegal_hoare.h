/** @file readerwriterprioegal_hoare.h
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
 * Le but ici est de gérer une prorité égale entre les lecteurs et
 * rédacteurs.
 */

#ifndef READERWRITERPRIOEGAL_HOARE_H
#define READERWRITERPRIOEGAL_HOARE_H

#include "ohoaremonitor.h"
#include "abstractreaderwriter.h"

class ReaderWriterPrioEgal_Hoare : public OHoareMonitor, public AbstractReaderWriter
{

protected:

    Condition attenteRedaction; //file attente pour les rédacteurs
    Condition attenteLecture; //file attente pour les lecteurs
    Condition fifo; //file attente pour attendre sur les autres files

    bool libre;
    int nbLecteurs;
    bool redactionEnCours;
    int nbRedacteursEnAttente;
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
     * @brief ReaderWriterPrioEgal_Hoare
     * constructeur de la ressource
     */
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

    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {

        monitorIn();

        //pour ne pas bruler la priorité
        if(!libre){
            wait(fifo);
        }
        libre = false;

        //si la lecture n'est pas possible, on attend
        if (redactionEnCours) {
            wait(attenteLecture);
            signal(attenteLecture);
        }
        nbLecteurs ++;

        libre = true;
        signal(fifo);

        monitorOut();
    }

    /**
     * @brief unlockReading
     * sortie de la ressource en lecture
     */
    void unlockReading() {

        monitorIn();

        nbLecteurs --;

        //le dernier lecteur signale passe la main aux rédacteurs
        if (nbLecteurs == 0) {
            signal(attenteRedaction);
        }

        libre = true;
        signal(fifo);

        monitorOut();
    }

    /**
     * @brief lockWriting
     * accès à la ressource en écriture
     */
    void lockWriting() {

        monitorIn();

        //pour ne pas bruler la priorité
        if(!libre){
            wait(fifo);
        }
        libre = false;

        //si l'écriture n'est pas possible, on attend
        if ((nbLecteurs > 0) || (redactionEnCours)) {
            nbRedacteursEnAttente ++;
            wait(attenteRedaction);
            nbRedacteursEnAttente --;
        }
        redactionEnCours = true;

        monitorOut();
    }

    /**
     * @brief unlockWriting
     * sortie de la ressource en écriture
     */
    void unlockWriting() {

        monitorIn();
        redactionEnCours = false;
        signal(attenteLecture);
        libre = true;
        signal(fifo);
        monitorOut();
    }





};

#endif // READERWRITERPRIOEGAL_HOARE_H
