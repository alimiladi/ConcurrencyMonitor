/** @file readerwriterprioreading_mesa.h
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
 * Le but ici est de gérer une prorité des lecteurs en lecture.
 * Nous avons remplacé les QWaitCondition par des OWaitCondition et
 * pareil pour QMutex. Ainsi nous garantissons un service de logs
 * sur la ressource et ses différentes files d'attente.
 */

#ifndef READERWRITERPRIOREADING_MESA_H
#define READERWRITERPRIOREADING_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"


class ReaderWriterPrioReading_Mesa :
        public AbstractReaderWriter {

protected:


    //un seul lecteur pourra attendre sur cette file avec les autres
    //rédacteurs
    OWaitCondition attenteTour;//fifo
    //les autres lecteurs doivent d'abord attendre sur cette file
    OWaitCondition attenteLecteurs;
    OMutex mutex;
    int nbLecteurs;
    bool redacteur;
    bool lecteurDansFifo;
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
     * @brief ReaderWriterPrioReading_Mesa
     * constructeur de la ressource
     */
    ReaderWriterPrioReading_Mesa() :
        nbLecteurs(0),
        redacteur(false),
        lecteurDansFifo(false),
        name("ReaderWriterPrioReading_Mesa")
    {
        attenteLecteurs.setName("attenteLecteurs");
        attenteTour.setName("attenteTour");
        mutex.setName("mutex");
    }

    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {
      mutex.lock();
      //si déjà un lecteur attend parmis les rédacteurs alors
      //les autres lecteurs attendront sur la file des lecteurs
      while(lecteurDansFifo){
          attenteLecteurs.wait(&mutex);
      }
      //le lecteur vérifie s'il n'y a pas de rédaction en cours sinon il attend
      //dans le fifo
      if(redacteur){
          lecteurDansFifo = true;
          attenteTour.wait(&mutex);
          lecteurDansFifo = false;
      }
      attenteLecteurs.wakeOne();
      nbLecteurs++;
      mutex.unlock();
    }

    /**
     * @brief unlockReading
     * sortie de la ressource en lecture
     */
    void unlockReading() {
      mutex.lock();
      nbLecteurs--;
      //le dernier lecteur réveil un thread du fifo
      if (nbLecteurs==0) {
        attenteTour.wakeOne();
      }
      //sinon on donne la priorité au lecteurs qui attendent
      else{
          attenteLecteurs.wakeOne();
      }
      mutex.unlock();
    }

    /**
     * @brief lockWriting
     * accès à la ressource en écriture
     */
    void lockWriting() {
        mutex.lock();
        //si la ressource est occupée, on attend su le fifo
        if (nbLecteurs > 0 || redacteur) {
            attenteTour.wait(&mutex);
        }
        redacteur = true;
        mutex.unlock();
    }

    /**
     * @brief unlockWriting
     * sortie de la ressource en écriture
     */
    void unlockWriting() {
      mutex.lock();
      redacteur = false;
      //on donne le tour au suivant dans le fifo
      attenteTour.wakeOne();
      mutex.unlock();
    }
};

#endif // READERWRITERPRIOREADING_MESA_H
