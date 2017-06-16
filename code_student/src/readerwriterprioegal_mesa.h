/** @file readerwriterprioegal_mesa.h
 *  @brief ressource accessible avec moniteur de mesa
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe représente une ressource que des lecteurs et rédacteurs
 * vondront accéder. Pour y accéder ou en sortir, les threads devront
 * appeler une des méthodes ci-dessous.
 * La ressource hérite pour cela d'une ressource abstraite
 * Le but ici est de gérer une prorité égale entre les lecteurs et
 * rédacteurs.
 * Nous avons remplacé les QWaitCondition par des OWaitCondition et
 * pareil pour QMutex. Ainsi nous garantissons un service de logs
 * sur la ressource et ses différentes files d'attente.
 */

#ifndef READERWRITERPRIOEGAL_MESA_H
#define READERWRITERPRIOEGAL_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"

class ReaderWriterPrioEgal_Mesa : public AbstractReaderWriter{

protected:

    //protège l'accès aux variables paratgées
    OMutex mutex;
    //file d'attente pour les rédacteurs après avoir passé le fifo
    OWaitCondition attenteEcriture;
    //nb lecteurs dans la ressource
    int nbLecture;
    //rédacteur dans la ressource
    bool ecriture;
    //nom de la ressource
    QString name;
    //file d'attente principale
    OWaitCondition fifo;
    //pour libérer un thread dans le fifo
    int libre;



public:

    /**
     * @brief getName
     * @return le nom de la ressource
     */
    QString getName(){
        return name;
    }


    /**
     * @brief ReaderWriterPrioEgal_Mesa
     * constructeur de la ressource
     */
    ReaderWriterPrioEgal_Mesa() :
        nbLecture(0),
        ecriture(false),
        name("Reader-Writer-PrioEgal_Mesa"),
        libre(true)
    {
        mutex.setName("mutex");
        attenteEcriture.setName("attenteEcriture");
        fifo.setName("fifo");
    }


    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {
        mutex.lock();

        //on attend dans le fifo temps que c'est pas le tour du prochain thread
        while(!libre){
            fifo.wait(&mutex);
        }
        //les suivants devront attendre dans le fifo
        libre = false;

        //lecteur dans la ressource
        nbLecture++;

        //on peut sortir un autre thread de la ressource
        //s'il s'agit d'un lecteur, alors il passera tout droit dans la ressource
        //sinon, s'il s'agit d'un rédacteur alors il attendra dans une seconde file
        //pour rédacteurs
        libre = true;
        fifo.wakeOne();

        mutex.unlock();
    }


    /**
     * @brief unlockReading
     * sortie de la ressource en lecture
     */
    void unlockReading() {
        mutex.lock();
        //on sort de la ressource
        nbLecture --;
        //si c'est le dernier lecteur à sortir
        if(nbLecture == 0){
            //on réveille un rédacteurs qui était déjà sorti du fifo
            attenteEcriture.wakeOne();
        }
        mutex.unlock();
    }

    /**
     * @brief lockWriting
     * accès à la ressource en écriture
     */
    void lockWriting() {
        mutex.lock();

        //on attend dans le fifo temps que c'est pas le tour du prochain thread
        while(!libre){
            fifo.wait(&mutex);
        }
        //les suivants devront attendre dans le fifo
        libre = false;

        //s'il y a des lecteurs dans la ressource alors on attend
        //sur une deuxième file dediée aux rédacteurs
        if(nbLecture > 0){
            attenteEcriture.wait(&mutex);
        }

        //le rédacteur est dans la ressource
        ecriture = true;

        mutex.unlock();

    }


    /**
     * @brief unlockWriting
     * sortie de la ressource en écriture
     */
    void unlockWriting() {
        mutex.lock();

        //le rédacteur quitte la ressource
        ecriture = false;

        //on donne le tour au prochain thread dans le fifo
        libre = true;
        fifo.wakeOne();

        mutex.unlock();
    }
};

#endif // READERWRITERPRIOEGAL_MESA_H
