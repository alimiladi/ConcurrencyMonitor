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

    Condition attenteRedaction;
    Condition attenteLecture;

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
     * @brief ReaderWriterPrioReading_Hoare
     * constructeur de la ressource
     */
    ReaderWriterPrioReading_Hoare() :
        nbLecteurs(0),
        redactionEnCours(false),
        nbRedacteursEnAttente(0),
        libre(true),
        name("ReaderWriterPrioReading_Hoare"){
        attenteLecture.setName("attenteLecture");
        attenteRedaction.setName("attenteRedaction");
    }

    /**
     * @brief lockReading
     * pour accéder à la ressource en lecture
     */
    void lockReading() {
        monitorIn();

        monitorOut();
    }

    /**
     * @brief unlockReading
     * pour sortir de la ressource en lecture
     */
    void unlockReading() {
        monitorIn();

        monitorOut();
    }

    /**
     * @brief lockWriting
     * pour accéder à la ressource en écriture
     */
    void lockWriting() {
        monitorIn();

        monitorOut();
    }

    /**
     * @brief unlockWriting
     * pour sortir de la ressource en écriture
     */
    void unlockWriting() {
        monitorIn();

        monitorOut();
    }
};

#endif // READERWRITERPRIOREADING_HOARE_H
