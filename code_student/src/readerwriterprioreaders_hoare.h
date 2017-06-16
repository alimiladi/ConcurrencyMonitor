/** @file readerwriterprioreaders_hoare.h
 *  @brief ressource accessible avec moniteur de hoare
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
 * Nous avons remplacé les Condition de la classe HoareMonitor par les
 * Condition de la classe OHoareMonitor. Ainsi nous garantissons un
 * service de logs sur la ressource et ses différentes files d'attente.
 */

#ifndef READERWRITERPRIOWRITERS_HOARE_H
#define READERWRITERPRIOWRITERS_HOARE_H

#include "abstractreaderwriter.h"
#include "ohoaremonitor.h"

class ReaderWriterPrioReaders_Hoare :
        public AbstractReaderWriter, public OHoareMonitor {
protected:

    Condition attenteRedaction;
    Condition attenteLecture;

    int nbLecteurs;
    bool redactionEnCours;
    int nbRedacteursEnAttente;
    int nbLecteursEnAttente;

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
     * @brief ReaderWriterPrioReaders_Hoare
     * construteur de la ressource
     */
    ReaderWriterPrioReaders_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        nbLecteursEnAttente(0),
        name("Reader-Writer-PrioReaders_Hoare"){
        attenteLecture.setName("attenteLecture");
        attenteRedaction.setName("attenteRedaction");
    }

    /**
     * @brief lockReading
     * accès à la ressource en lecture
     */
    void lockReading() {
        // La seule condition pour ne pas rentrer pour un lecteur est qu'il n'y a pas de rédaction en cours
        monitorIn();
        if (redactionEnCours) {
            // Attente seulement si rédaction en cours
            nbLecteursEnAttente ++;
            wait(attenteLecture);
            // Moniteur de Hoare donc acquisition du mutex juste après le réveil du wait
            nbLecteursEnAttente --;
        }
        nbLecteurs ++;
        monitorOut();
    }

    /**
     * @brief unlockReading
     * sortie de la ressource en lecture
     */
    void unlockReading() {
        // A la sortie de la rerssource, contrôle si c'est le dernier lecteur
        monitorIn();
        //Décrément du nombre de lecteurs
        nbLecteurs --;
        if (!nbLecteurs && nbRedacteursEnAttente) {
                //Si c'est le dernier, il réveille un potentiel rédacteur s'il y'en a en attente
                signal(attenteRedaction);
        }
        monitorOut();
    }

    /**
     * @brief lockWriting
     * accès à la resource en écriture
     */
    void lockWriting() {
        // Pour un rédacteur, il doit d'abord vérifier qu'il n'y a pas d'autres rédacteurs en cours d'écriture,
        // ensuite pas de lecteurs en cours de lecture ni en attente de lecture
        monitorIn();
        if (nbLecteurs || nbLecteursEnAttente || redactionEnCours) {
            nbRedacteursEnAttente ++;
            // Moniteur de Hoare donc certitude de réacquérir le mutex après le wait
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
        // A la sortie de la ressource, un rédacteur doit contrôler s'il y'a des lecteurs en attente sur celle-ci
        monitorIn();
        redactionEnCours = false;
        if (nbLecteursEnAttente) {
            // Dans le cas favorable, il en réveille un
            signal(attenteLecture);
        }
        else if (nbRedacteursEnAttente){
            //Dans le cas conttraire et s'il y'a des rédateurs en attente, il en réveille un
            signal(attenteRedaction);
        }
        monitorOut();
    }
};

#endif // READERWRITERPRIOWRITERS_HOARE_H
