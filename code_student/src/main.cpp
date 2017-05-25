
#include <QApplication>
#include "synchrocontroller.h"
#include "mythread.h"
#include <QDebug>

#define NB_THREADS_READER 10
#define NB_THREADS_WRITER 10

int main(int argc, char *argv[])
{

    //création du resource manager object



    //création des threads lecteurs
    QList<MyThread*> *readerThreadsList = new QList<MyThread*>();
    for(int i = 0; i < NB_THREADS_READER; i++){
        readerThreadsList->append(new MyThread("L"+(QString)i));
    }


    //création des threads rédacteurs
    QList<MyThread*> *writerThreadsList = new QList<MyThread*>();
    for(int i = 0; i < NB_THREADS_WRITER; i++){
        writerThreadsList->append(new MyThread("R"+(QString)i));
    }



    std::cout << "-----------LANCEMENT SCENARIO----------" << std::endl;
    //préparation pour le début du scénario arrivées lecteurs/rédacteurs
    int nbWritersLaunched = 0;
    int nbReadersLaunched = 0;
    bool continuing = true;

    while (continuing) {

        //s'il reste assez de lecteurs et de rédacteurs pour en chosir
        //au hasard entre les deux
        if(nbWritersLaunched < NB_THREADS_WRITER &&
                nbReadersLaunched < NB_THREADS_READER){

            //on lance au hasard soit un rédacteur, soit un lecteur
            int randNum = rand() % 2;

            //s'il faut lancer un rédacteur
            if(randNum == 0){
                //on lance le prochain rédacteur
                writerThreadsList->at(nbWritersLaunched)->start();
                nbWritersLaunched++;
            }
            //autrement on lance un lecteur
            else{
                //on lance le prochain lecteur
                readerThreadsList->at(nbReadersLaunched)->start();
                nbReadersLaunched++;
            }
        }
        //autrement s'il ne reste plus que des rédacteurs
        //on lance des rédacteurs
        else if(nbWritersLaunched < NB_THREADS_WRITER &&
                nbReadersLaunched == NB_THREADS_READER){

            //on lance le prochain rédacteur
            writerThreadsList->at(nbWritersLaunched)->start();
            nbWritersLaunched++;
        }
        //autrement s'il ne reste plus que des lecteurs
        //on lance des lecteurs
        else if(nbWritersLaunched == NB_THREADS_WRITER &&
                nbReadersLaunched < NB_THREADS_READER){

            //on lance le prochain rédacteur
            readerThreadsList->at(nbReadersLaunched)->start();
            nbReadersLaunched++;
        }
        //autrement il ne reste plus personne à lancer
        //alors on termine le scénario
        else{
            continuing = false;
        }


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
