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

    OMutex mutex;
    OWaitCondition attenteLecture;
    int nbAttenteLecture;
    OWaitCondition attenteEcriture;
    int nbAttenteEcriture;
    int nbLecture;
    bool ecriture;
    QString name;
    OWaitCondition fifo;
    int libre;
    int nbAttenteFifo;



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
        nbAttenteLecture(0),
        nbAttenteEcriture(0),
        nbLecture(0),
        ecriture(false),
        name("Reader-Writer-PrioEgal_Mesa"),
        libre(true),
        nbAttenteFifo(0)
    {
        mutex.setName("mutex");
        attenteLecture.setName("attenteLecture");
        attenteEcriture.setName("attenteEcriture");
        fifo.setName("fifo");
    }


    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {
        mutex.lock();

        while(!libre){
            fifo.wait(&mutex);

        }
        libre = false;

        nbLecture++;

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
        nbLecture --;
        if(nbLecture == 0){
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

        while(!libre){
            fifo.wait(&mutex);
        }
        libre = false;

        if(nbLecture > 0){
            attenteEcriture.wait(&mutex);
        }

        ecriture = true;

        mutex.unlock();

    }


    /**
     * @brief unlockWriting
     * sortie de la ressource en écriture
     */
    void unlockWriting() {
        mutex.lock();
        ecriture = false;
        libre = true;
        fifo.wakeOne();
        mutex.unlock();
    }
};

#endif // READERWRITERPRIOEGAL_MESA_H
