
#include <QApplication>
#include "synchrocontroller.h"
#include "taskwriter.h"
#include "taskreader.h"
#include <QDebug>

//les 4 types (priorités) de ressources partagées avec semaphore
#include "readerwriterequal_sem.h"
#include "readerwriterprioreaders_sem.h"
#include "readerwriterprioreading_sem.h"
#include "readerwriterpriowriter_sem.h"

#define NB_THREADS_READER 10
#define NB_THREADS_WRITER 10

int main(int argc, char *argv[])
{

    //création du synchro controleur
    SynchroController* synchroController = SynchroController::getInstance();

    //création de la ressource partagée
    //ReaderWriterEqual_Sem *resource;
    //ReaderWriterPrioReaders_Sem *resource;
    //ReaderWriterPrioReading_Sem *resource;
    ReaderWriterPrioWriter_Sem *resource;

    //création des threads lecteurs
    /*QList<TaskReader*> *readersList = new QList<TaskReader*>();
    for(int i = 0; i < NB_THREADS_READER; i++){
        readersList->append(new TaskReader(i, "taskReader"+i, resource));
        readersList->at(i)->start();
    }

    //création des threads rédacteurs
    QList<TaskWriter*> *writersList = new QList<TaskWriter*>();
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writersList->append(new TaskWriter(i, "taskWriter"+i, resource));
        writersList->at(i)->start();
    }*/


    std::cout << "-----------LANCEMENT SCENARIO----------" << std::endl;

    bool continuing = true;
    char saisie;
    bool ko;
    while (continuing) {

        //on attend la pause d'un thread
        synchroController->getMainWaiting()->acquire();

        // Wait for a key press
        do {
           std::cout << "Do you want to continue? (y or n):";
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
            std::cout << "encore" << std::endl;
            SynchroController::getInstance()->resume();
        }
        // If key was <n>
        else if(saisie == 'n'){
            std::cout << "fin" << std::endl;
            continuing = false;
        }
    }



    //terminaison des threads Reader
   /* for(int i = 0; i < NB_THREADS_READER; i++){
        readersList->at(i)->exit();
    }

    //terminaison des threads Writer
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writersList->at(i)->exit();
    }*/


    return 0;



    // Pour ceux qui voudraient développer une version graphique
    //    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();

    //    return a.exec();
}
