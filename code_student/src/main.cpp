
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

    //création du resource manager object

    //création de la ressource partagée
    //ReaderWriterEqual_Sem *resource;
    //ReaderWriterPrioReaders_Sem *resource;
    //ReaderWriterPrioReading_Sem *resource;
    ReaderWriterPrioWriter_Sem *resource;

    //création des threads lecteurs
    QList<TaskReader*> *readersList = new QList<TaskReader*>();
    for(int i = 0; i < NB_THREADS_READER; i++){
        readersList->append(new TaskReader(i, "taskReader"+i, resource));
        readersList->at(i)->start();
    }

    //création des threads rédacteurs
    QList<TaskWriter*> *writersList = new QList<TaskWriter*>();
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writersList->append(new TaskWriter(i, "taskWriter"+i, resource));
        writersList->at(i)->start();
    }


    std::cout << "-----------LANCEMENT SCENARIO----------" << std::endl;

    bool continuing = true;
    while (continuing) {
        // Wait for a key press

        // If key is <enter>
        SynchroController::getInstance()->resume();

        // If key was <esc>
        continuing = false;
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
