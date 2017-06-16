
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


    //on récupère le waitinglogger
    WaitingLogger* waitingLogger = WaitingLogger::getInstance();



    //création de la ressource partagée avec sémaphores
    //ReaderWriterEqual_Sem *resource = new ReaderWriterEqual_Sem();
    //ReaderWriterPrioReaders_Sem *resource = new ReaderWriterPrioReaders_Sem();
    //ReaderWriterPrioReading_Sem *resource = new ReaderWriterPrioReading_Sem();
    //ReaderWriterPrioWriter_Sem *resource = new ReaderWriterPrioWriter_Sem();

    //création de la ressource partagée avec Mesa
    //ReaderWriterPrioWriter_Mesa *resource = new ReaderWriterPrioWriter_Mesa();
    //fonctionne pas:
    ReaderWriterPrioEgal_Mesa *resource = new ReaderWriterPrioEgal_Mesa();
    //ReaderWriterPrioReaders_Mesa *resource = new ReaderWriterPrioReaders_Mesa();
    //ReaderWriterPrioReading_Mesa *resource = new ReaderWriterPrioReading_Mesa();

    //création de la ressource partagée avec Hoare
    //ReaderWriterPrioWriter_Hoare *resource = new ReaderWriterPrioWriter_Hoare();
    //trop de files d'attente:
    //ReaderWriterPrioEgal_Hoare *resource = new ReaderWriterPrioEgal_Hoare();
    //fonctionne pas
    //ReaderWriterPrioReading_Hoare *resource = new ReaderWriterPrioReading_Hoare();



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

    bool continuing = true;
    char saisie;
    bool ko;
    int compteurLecteurId = 0;
    int compteurRedacteurId = 0;
    while (continuing) {

       std::cout.flush();
       saisie = std::cin.get();

       if(saisie == 'q'){
           std::cout << "fin" << std::endl;
           continuing = false;
           exit(0);
       }
       else{

            if(compteurLecteurId < NB_THREADS_READER && compteurRedacteurId < NB_THREADS_WRITER){
                // on donne le choix d'exécuter un lecteur ou un rédacteur
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

                if(saisie == 'l'){
                    std::cout << "On lance le lecteur" << compteurLecteurId << "!" << std::endl;
                    readersList->at(compteurLecteurId)->start();
                    compteurLecteurId++;
                }
                else if(saisie == 'r'){
                    std::cout << "On lance le redacteur" << compteurRedacteurId << "!" << std::endl;
                    writersList->at(compteurRedacteurId)->start();
                    compteurRedacteurId++;
                }
            }
            //autrement s'il ne reste que des lecteurs à lancer
            else if(compteurLecteurId < NB_THREADS_READER){
                std::cout << "On lance le lecteur" << compteurLecteurId << "!" << std::endl;
                readersList->at(compteurLecteurId)->start();
                compteurLecteurId++;
            }
            //autrement s'il ne reste que des rédacteurs à lancer
            else if(compteurRedacteurId < NB_THREADS_WRITER){
                std::cout << "On lance le redacteur" << compteurRedacteurId << "!" << std::endl;
                writersList->at(compteurRedacteurId)->start();
                compteurRedacteurId++;
            }

            SynchroController::getInstance()->resume();
        }

        //on attend la pause d'un thread
        synchroController->getMainWaiting()->acquire();

    }



    return 0;
}
