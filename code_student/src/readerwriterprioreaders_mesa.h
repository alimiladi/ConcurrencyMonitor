/** @file readerwriterprioreaders_mesa.h
 *  @brief ressource accessible avec moniteur de mesa
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe représente une ressource que des lecteurs et rédacteurs
 * voudront accéder. Pour y accéder ou en sortir, les threads devront
 * appeler une des méthodes ci-dessous.
 * La ressource hérite pour cela d'une ressource abstraite
 * Le but ici est de gérer une prorité des lecteurs.
 * Nous avons remplacé les QWaitCondition par des OWaitCondition et
 * pareil pour QMutex. Ainsi nous garantissons un service de logs
 * sur la ressource et ses différentes files d'attente.
 */

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

    /**
     * @brief getName
     * @return le nom de la ressource
     */
    QString getName(){
        return name;
    }

    /**
     * @brief ReaderWriterPrioReaders_Mesa
     * constructeur de la classe
     */
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


    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {
        mutex.lock();
        //Attente sur la condition seulement si un rédacteur est déja dans la ressource
        while (redactionEnCours) {
            nbLecteursEnAttente ++;
            attenteLecture.wait(&mutex);
            nbLecteursEnAttente --;
        }
        //Incrémentation du nombre de lecteurs de toute façon car soit pas de rédaction en cours
        // soit réception d'un wakeOne ==> déblocage du wait
        nbLecteurs ++;
        mutex.unlock();
    }

    /**
     * @brief unlockReading
     * sortie de la resource en lecture
     */
    void unlockReading() {
        mutex.lock();
        nbLecteurs --;
        //Réveil d'un rédacteur seulement si c'est le dernier des lecteurs
        if (!nbLecteurs && nbRedacteursEnAttente) {
            attenteEcriture.wakeOne();
        }
//        else
//            //Si encore des lecteurs en attente lors de la sortie de la resource, en libérer un
//            attenteLecture.wakeOne();
        mutex.unlock();
    }

    /**
     * @brief lockWriting
     * accès à la ressource en écriture
     */
    void lockWriting() {
        mutex.lock();
        //Attente sur la condition dans le cas ou au moins un lecteur est en cours
        //de lecture ou en attente de lecture
        // ou qu'un autre rédacteur est en train d'écrire.
        if ( nbLecteurs || nbLecteursEnAttente || redactionEnCours ) {            
            nbRedacteursEnAttente ++;
            attenteEcriture.wait(&mutex);
            nbRedacteursEnAttente --;
        }
        redactionEnCours = true;
        mutex.unlock();
    }


    /**
     * @brief unlockWriting
     * sortie de la ressource en écriture
     */
    void unlockWriting() {
        mutex.lock();
        redactionEnCours = false;
        //Libération d'un lecteur si au moins un en attente
        if (nbLecteursEnAttente) {
            attenteLecture.wakeOne();
            mutex.unlock();
        }
        //Si pas de lecteurs en attente mais rédacteurs en attente, en libérer un
        else if (nbRedacteursEnAttente) {
            attenteEcriture.wakeOne();
            mutex.unlock();
        }
    }
};

#endif // READERWRITERPRIOREADERS_MESA_H
