
#include <QApplication>
#include "synchrocontroller.h"
#include "taskwriter.h"
#include <QDebug>

#define NB_THREADS_READER 10
#define NB_THREADS_WRITER 10

int main(int argc, char *argv[])
{

    //création du resource manager object

    //création des threads lecteurs

    //création des threads rédacteurs

    std::cout << "-----------LANCEMENT SCENARIO----------" << std::endl;
    bool continuing = true;

    while (continuing) {
        // Wait for a key press

        // If key is <enter>
        SynchroController::getInstance()->resume();

        // If key was <esc>
        continuing = false;
    }





    //terminaison des threads
    /*for(int i = 0; i < NB_THREADS_READER+NB_THREADS_WRITER; i++){

        writerThreadsList->at(i)->exit();
        readerThreadsList->at(i)->exit();
    }*/



    /* Attends la fin de chaque thread et libère la mémoire associée.
    * Durant l'attente, l'application est bloquée.
    */

    /*MyThread *currentThread;
       for (int i=0; i< NB_THREADS_READER+NB_THREADS_WRITER; i++){
           currentThread = readerThreadsList->at(i);
           currentThread->wait();
           delete currentThread;
           currentThread = writerThreadsList->at(i);
           currentThread->wait();
           delete currentThread;
       }*/




    // Kill the threads

    return 0;



    // Pour ceux qui voudraient développer une version graphique
    //    QApplication a(argc, argv);
    //    MainWindow w;
    //    w.show();

    //    return a.exec();
}
