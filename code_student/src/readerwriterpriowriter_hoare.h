/** @file readerwriterpriowriter_hoare.h
 *  @brief ressource accessible avec moniteur de hoare
 *
 *  @author Yann Thoma : modifié par Ali Miladi, Quentin Zeller,
 *  Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Cette classe représente une ressource que des lecteurs et rédateurs
 * vondront accéder. Pour y accéder ou en sortir, les threads devront
 * appeler une des méthodes ci-dessous.
 * La ressource hérite pour cela d'une ressource abstraite et du moniteur
 * de hoare fournissant un service de logs.
 * Le but ici est de gérer une prorité pour les rédacteurs.
 * Cette classe est reprise du professeur. Pour cela nous n'ajouterons pas plus
 * de commentaires ou modifications si ce n'est le passage de HoareMoniteur à
 * OHoareMoniteur, permettant ainsi la journalisation de logs sur les différentes files.
 * Nous avons, par conséquent, aussi du nommer ces différentes files
 * ainsi que cette ressource.
 */

#ifndef READERWRITERPRIOWRITER_HOARE_H
#define READERWRITERPRIOWRITER_HOARE_H
#include "abstractreaderwriter.h"
#include "ohoaremonitor.h"

class ReaderWriterPrioWriter_Hoare :
        public AbstractReaderWriter, public OHoareMonitor {
protected:

    Condition attenteRedaction;
    Condition attenteLecture;

    int nbLecteurs;
    bool redactionEnCours;
    int nbRedacteursEnAttente;

    QString name;

public:

    QString getName(){
        return name;
    }

    ReaderWriterPrioWriter_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        name("Reader-Writer-PrioWriter_Hoare"){
        attenteLecture.setName("attenteLecture");
        attenteRedaction.setName("attenteRedaction");
    }

    void lockReading() {
        monitorIn();
        if ((redactionEnCours) || (nbRedacteursEnAttente > 0)) {
            wait(attenteLecture);
            signal(attenteLecture);
        }
        nbLecteurs ++;
        monitorOut();
    }

    void unlockReading() {
        monitorIn();
        nbLecteurs --;
        if (nbLecteurs == 0) {
            signal(attenteRedaction);
        }
        monitorOut();
    }

    void lockWriting() {
        monitorIn();
        if ((nbLecteurs > 0) || (redactionEnCours)) {
            nbRedacteursEnAttente ++;
            wait(attenteRedaction);
            nbRedacteursEnAttente --;
        }
        redactionEnCours = true;
        monitorOut();
    }

    void unlockWriting() {
        monitorIn();
        redactionEnCours = false;
        if (nbRedacteursEnAttente > 0)
            signal(attenteRedaction);
        else
            signal(attenteLecture);
        monitorOut();
    }
};



#endif // READERWRITERPRIOWRITER_HOARE_H
