/** @file main.cpp
 *  @brief programme principal
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug pas de bugs connus
 *
 * Ce programme génère un scénario de différents threads de type
 * lecteurs ou rédacteurs qui veulent accéder à une ressource partagée.
 * Le programme offre un service de logs qui décrivent l'état de la
 * ressource pour chaque étape. Les étapes sont marquées par des pauses.
 * Une saisie utilisateur va permettre de relancer les threads
 * (lecteurs/rédacteurs) et de mettre en pause le programme principal jusqu'à
 * l'affichage des prochain logs et la mise en pause du thread en exécution.
 * Ce programme regroupe donc différents objets tels que la ressource
 * (plusieurs types sont à disposition), le synchro contrôleur (gestion des pauses
 * et départs), le contrôleur de logs et les threads lecteurs/rédacteurs.
 *
 * REMARQUE IMPORTANTE: LA FILE D'ATTENTE ET SON LOG CORRESPONDANT NE SONT
 * PAS GENERES TANT JUSQU'AU PREMIER THREAD QUI ATTENDRA DESSUS.
 * Il est donc normal que les files ne soient pas toutes affichées dans les
 * logs au lancement du programme. Il faut attendre que ces dernières se
 * construisent pour pouvoir les voir.
 */

#include <QApplication>
#include "synchrocontroller.h"
#include "reader_writer_thread.h"
#include <QDebug>
#include "waitinglogger.h"

//les 4 types (priorités) de ressources partagées avec semaphore
#include "readerwriterequal_sem.h"
#include "readerwriterprioreaders_sem.h"
#include "readerwriterprioreading_sem.h"
#include "readerwriterpriowriter_sem.h"

//les 4 types (priorités) de ressources partagées avec Mesa
#include "readerwriterpriowriter_mesa.h"
#include "readerwriterprioegal_mesa.h"
#include "readerwriterprioreaders_mesa.h"
#include "readerwriterprioreading_mesa.h"

//les 4 types (priorités) de ressources partagées avec Hoare
#include "readerwriterpriowriter_hoare.h"
#include "readerwriterprioegal_hoare.h"
#include "readerwriterprioreading_hoare.h"

#define NB_THREADS_READER 3
#define NB_THREADS_WRITER 3

int main(int argc, char *argv[])
{

    //on récupère le synchrocontroller
    SynchroController* synchroController = SynchroController::getInstance();


    //création de la ressource partagée avec sémaphores
    //ReaderWriterEqual_Sem *resource = new ReaderWriterEqual_Sem();
    //ReaderWriterPrioReaders_Sem *resource = new ReaderWriterPrioReaders_Sem();
    //ReaderWriterPrioReading_Sem *resource = new ReaderWriterPrioReading_Sem();
    //ReaderWriterPrioWriter_Sem *resource = new ReaderWriterPrioWriter_Sem();

    //création de la ressource partagée avec Mesa
    //ReaderWriterPrioWriter_Mesa *resource = new ReaderWriterPrioWriter_Mesa();
    //ReaderWriterPrioEgal_Mesa *resource = new ReaderWriterPrioEgal_Mesa();
    //ReaderWriterPrioReaders_Mesa *resource = new ReaderWriterPrioReaders_Mesa();
    //ReaderWriterPrioReading_Mesa *resource = new ReaderWriterPrioReading_Mesa();

    //création de la ressource partagée avec Hoare
    //ReaderWriterPrioWriter_Hoare *resource = new ReaderWriterPrioWriter_Hoare();
    //ReaderWriterPrioEgal_Hoare *resource = new ReaderWriterPrioEgal_Hoare();
    //fonctionne pas
    ReaderWriterPrioReading_Hoare *resource = new ReaderWriterPrioReading_Hoare();



    //on prépare le compteur pour les identifiants unique de logs
    //entre tous les threads (lecteur et rédacteurs)
    unsigned int logId = 0;


    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "SCENARIO: "<< qPrintable(resource->getName()) << std::endl;
    std::cout << "REDACTEURS: " << NB_THREADS_WRITER << std::endl;
    std::cout << "LECTEURS: " << NB_THREADS_READER << std::endl;
    std::cout << "Pressez <enter> pour continuer ou <q> pour quitter" << std::endl;
    std::cout << "-----------------------------------------------------------------------\n" << std::endl;


    //création des threads rédacteurs
    QList<TaskWriter*> *writersList = new QList<TaskWriter*>();
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writersList->append(new TaskWriter(logId, QString("redacteur%1").arg(i), resource));
    }

    //création des threads lecteurs
    QList<TaskReader*> *readersList = new QList<TaskReader*>();
    for(int i = 0; i < NB_THREADS_READER; i++){
        readersList->append(new TaskReader(logId, QString("lecteur%1").arg(i), resource));
    }

    //variables pour la saisie et le lancement des threads
    bool continuing = true;
    char saisie;
    bool ko; //si mauvaise saisie
    int compteurLecteurId = 0;
    int compteurRedacteurId = 0;

    //tant que l'utilisateur veut continuer
    while (continuing) {

        //vide le flux
       std::cout.flush();

       //récupère saisie
       saisie = std::cin.get();

       //si l'utilisateur veut terminer le programme, on le termine
       if(saisie == 'q'){
           std::cout << "fin" << std::endl;
           continuing = false;
           exit(0);
       }
       //autrement on le poursuit
       else{

           //s'il reste des lecteurs et rédacteurs à lancer alors on laisse le choix à l'utilisateur
            if(compteurLecteurId < NB_THREADS_READER && compteurRedacteurId < NB_THREADS_WRITER){

                //saisie utilisateur
                do {
                   std::cout << "Lecteur ou redacteur? (l ou r): ";
                   std::cout.flush();
                   std::cin >> saisie;
                   ko = std::cin.fail() || (saisie != 'l' && saisie != 'r');
                   if (std::cin.fail()) {
                      std::cin.clear();
                      std::cout << "input error" << endl;
                   }
                   while(std::cin.get() != '\n'); // vide le buffer
                } while (ko);

                //si on lecteur est choisit, on le lance
                if(saisie == 'l'){
                    std::cout << "On lance le lecteur" << compteurLecteurId << "!" << std::endl;
                    readersList->at(compteurLecteurId)->start();
                    compteurLecteurId++;
                }
                //autrement si on rédacteur est choisit, on le lance
                else if(saisie == 'r'){
                    std::cout << "On lance le redacteur" << compteurRedacteurId << "!" << std::endl;
                    writersList->at(compteurRedacteurId)->start();
                    compteurRedacteurId++;
                }
            }
            //autrement s'il ne reste que des lecteurs à lancer, on lance automatiquement un lecteur
            else if(compteurLecteurId < NB_THREADS_READER){
                std::cout << "On lance le lecteur" << compteurLecteurId << "!" << std::endl;
                readersList->at(compteurLecteurId)->start();
                compteurLecteurId++;
            }
            //autrement s'il ne reste que des rédacteurs à lancer, on lance automatiquement un rédacteur
            else if(compteurRedacteurId < NB_THREADS_WRITER){
                std::cout << "On lance le redacteur" << compteurRedacteurId << "!" << std::endl;
                writersList->at(compteurRedacteurId)->start();
                compteurRedacteurId++;
            }

            //on libère un thread lecteur ou rédacteur
            synchroController->resume();
        }

        //on attend la pause d'un thread lecteur ou rédacteur
        synchroController->getMainWaiting()->acquire();

    }

    return 0;
}
