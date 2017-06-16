/** @file readerwriterpriowriter_mesa.h
 *  @brief ressource accessible avec moniteur de mesa
 *
 *  @author Yann Thoma : modifié par Ali Miladi, Quentin Zeller,
 *  Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe représente une ressource que des lecteurs et rédateurs
 * vondront accéder. Pour y accéder ou en sortir, les threads devront
 * appeler une des méthodes ci-dessous.
 * La ressource hérite pour cela d'une ressource abstraite.
 * Le but ici est de gérer une prorité pour les rédacteurs.
 * Cette classe est reprise du professeur. Pour cela nous n'ajouterons pas plus
 * de commentaires ou modifications si ce n'est le passage de QWaitCondition à
 * OWaitCondition et QMutex à OMutex, permettant ainsi la journalisation de logs
 * sur les différentes files.
 * Nous avons, par conséquent, aussi du nommer ces différentes files
 * ainsi que cette ressource.
 */

#ifndef READERWRITERPRIOWRITER_MESA_H
#define READERWRITERPRIOWRITER_MESA_H

#include "owaitcondition.h"
#include "abstractreaderwriter.h"


class ReaderWriterPrioWriter_Mesa :
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

    ReaderWriterPrioWriter_Mesa() :
        nbLecteurs(0),
        nbLecteursEnAttente(0),
        nbRedacteursEnAttente(0),
        redactionEnCours(false),
        name("Reader-Writer-PrioWriter_Mesa")
    {
        attenteEcriture.setName("attenteEcriture");
        attenteLecture.setName("attenteLecture");
        mutex.setName("mutex");
    }

    void lockReading() {
        mutex.lock();
        if ((nbRedacteursEnAttente > 0) || (redactionEnCours)) {
            nbLecteursEnAttente ++;
            attenteLecture.wait(&mutex);
        }
        else
            nbLecteurs ++;
        mutex.unlock();
    }

    void unlockReading() {
        mutex.lock();
        nbLecteurs --;
        if (nbLecteurs == 0) {
            attenteEcriture.wakeOne();
        }
        mutex.unlock();
    }

    void lockWriting() {
        mutex.lock();
        if ((nbLecteurs > 0) || (redactionEnCours)) {
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
        if (nbRedacteursEnAttente) {
            nbRedacteursEnAttente --;
            redactionEnCours = true;
            attenteEcriture.wakeOne();
        }
        else {
            for(int i=0;i<nbLecteursEnAttente; i++) {
                attenteLecture.wakeOne();
            }
            nbLecteurs = nbLecteursEnAttente;
            nbLecteursEnAttente = 0;

        }
        mutex.unlock();
    }
};


#endif // READERWRITERPRIOWRITER_MESA_H
