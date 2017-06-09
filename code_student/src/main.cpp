
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

//les 4 types (priorités) de ressources partagées avec Hoare
#include "readerwriterpriowriter_hoare.h"

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
//    ReaderWriterPrioWriter_Mesa *resource = new ReaderWriterPrioWriter_Mesa();

    //création de la ressource partagée avec Hoare
    ReaderWriterPrioWriter_Hoare *resource = new ReaderWriterPrioWriter_Hoare();


    //on set le tableau des logs
    waitingLogger->setSizeLogs(NB_THREADS_READER+NB_THREADS_WRITER);

    //on prépare le compteur pour les identifiants unique de logs
    //entre tous les threads (lecteur et rédacteurs)
    unsigned int logId = 0;


    std::cout << "-----------------------------------------------------------------------" << std::endl;
    std::cout << "----------------SCENARIO: "<< qPrintable(resource->getName()) << "----------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------" << std::endl;


    //création des threads rédacteurs
    QList<TaskWriter*> *writersList = new QList<TaskWriter*>();
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writersList->append(new TaskWriter(logId, QString("redacteur%1").arg(i), resource));
        writersList->at(i)->start();
        logId++;
    }

    //création des threads lecteurs
    QList<TaskReader*> *readersList = new QList<TaskReader*>();
    for(int i = 0; i < NB_THREADS_READER; i++){
        readersList->append(new TaskReader(logId, QString("lecteur%1").arg(i), resource));
        readersList->at(i)->start();
        logId++;
    }

    bool continuing = true;
    char saisie;
    bool ko;
    while (continuing) {

        //on attend la pause d'un thread
        synchroController->getMainWaiting()->acquire();

        // Wait for a key press
        do {

           std::cout << "Do you want to continue? (y or n): ";
           std::cout.flush();
           std::cin >> saisie;
           ko = std::cin.fail() || (saisie != 'y' && saisie != 'n');
           if (std::cin.fail()) {
              std::cin.clear();
              std::cout << "input error" << endl;
           }
           while(std::cin.get() != '\n'); // vide le buffer
        } while (ko);

        // If key is <y>
        if(saisie == 'y'){

            //on efface tous les logs
            //si on efface depuis ici, les logs seront plus détaillés mais il y aura des étapes vide
            //c'est péférable d'utiliser le Waitinglogger et sa méthode updateView pour effacer les logs
            //à chaque fois qu'on veut en écrire un nouveau. Même si cela procure parfois l'effet d'étape sautées
            //pour l'utilisateur qui regarde les logs. Nous avons constaté que cela arrive très rarement.
            /*for(int i = 0 ; i < NB_THREADS_READER+NB_THREADS_WRITER; i++){
                WaitingLogger::getInstance()->clearLogs(i);
            }*/

            SynchroController::getInstance()->resume();
        }
        // If key was <n>
        else if(saisie == 'n'){
            std::cout << "fin" << std::endl;
            continuing = false;
        }
    }



    //terminaison des threads Reader
    for(int i = 0; i < NB_THREADS_READER; i++){
        readersList->at(i)->exit();
    }

    //terminaison des threads Writer
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writersList->at(i)->exit();
    }


    return 0;



    // Pour ceux qui voudraient développer une version graphique
    //    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();

    //    return a.exec();
}
