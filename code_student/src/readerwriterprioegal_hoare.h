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

    //file d'attente pour les rédacteurs après avoir passé le fifo
    Condition attenteRedaction;
    //file d'attente principale, pour ne pas bruler la priorité
    Condition fifo;
    //pour libérer un thread dans le fifo
    bool libre;
    //nb lecteurs dans la ressource
    int nbLecteurs;
    //rédacteur dans la ressource
    bool redactionEnCours;
    //nom de la ressource
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
        libre(true),
        name("Reader-Writer-PrioEgal_Hoare"){
        attenteRedaction.setName("attenteRedaction");
        fifo.setName("fifo");
    }

    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {

        monitorIn();

        //on attend dans le fifo temps que c'est pas le tour du prochain thread
        if(!libre){
            wait(fifo);
        }
        //les suivants devront attendre dans le fifo
        libre = false;

        //lecteur dans la ressource
        nbLecteurs ++;

        //on peut sortir un autre thread de la ressource
        //s'il s'agit d'un lecteur, alors il passera tout droit dans la ressource
        //sinon, s'il s'agit d'un rédacteur alors il attendra dans une seconde file
        //pour rédacteurs
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

        //on sort de la ressource
        nbLecteurs --;

        //si c'est le dernier lecteur à sortir
        if (nbLecteurs == 0) {
            //on réveille un rédacteurs qui était déjà sorti du fifo
            signal(attenteRedaction);
        }

        monitorOut();
    }

    /**
     * @brief lockWriting
     * accès à la ressource en écriture
     */
    void lockWriting() {

        monitorIn();

        //on attend dans le fifo temps que c'est pas le tour du prochain thread
        if(!libre){
            wait(fifo);
        }
        //les suivants devront attendre dans le fifo
        libre = false;

        //s'il y a des lecteurs dans la ressource alors on attend
        //sur une deuxième file dediée aux rédacteurs
        if (nbLecteurs > 0) {
            wait(attenteRedaction);
        }

        //le rédacteur est dans la ressource
        redactionEnCours = true;

        monitorOut();
    }

    /**
     * @brief unlockWriting
     * sortie de la ressource en écriture
     */
    void unlockWriting() {

        monitorIn();

        //le rédacteur quitte la ressource
        redactionEnCours = false;

        //on donne le tour au prochain thread dans le fifo
        libre = true;
        signal(fifo);

        monitorOut();
    }





};

#endif // READERWRITERPRIOEGAL_HOARE_H
