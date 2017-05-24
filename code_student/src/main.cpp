
#include <QApplication>
#include "synchrocontroller.h"
#include "mythread.h"

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



    //préparation pour le début du scénario arrivées lecteurs/rédacteurs
    int nbWritersLaunched = 0;
    int nbReadersLaunched = 0;
    bool continuing = true;

    while (continuing) {

        // Wait for a key press

        // If key is <enter>
        SynchroController::getInstance()->resume();


        // If key was <esc>
        continuing = false;


        //on lance au hasard soit un rédacteur, soit un lecteur
        int randNum = rand() % 2;

        //s'il faut lancer un rédacteur
        if(randNum == 0){
            //et qu'il y a encore des rédacteurs à lancer
            if(nbWritersLaunched < NB_THREADS_WRITER){
                //on lance le prochain rédacteur
                writerThreadsList->at(nbWritersLaunched)->start();
                nbWritersLaunched++;

            }
            //s'il n'y a plus de rédacteurs alors on lance un lecteur,
            //s'il en reste
            else if(nbReadersLaunched < NB_THREADS_READER){
                //on lance le prochain lecteur
                readerThreadsList->at(nbReadersLaunched)->start();
                nbReadersLaunched++;
            }
            //s'il ne reste plus personne alors on arrête
            else{
                continuing = false;
            }
        }
        //autrement s'il faut lancer un rédacteur
        else{
            //et qu'il y a encore des lecteurs à lancer
            if(nbReadersLaunched < NB_THREADS_READER){
                //on lance le prochain lecteur
                readerThreadsList->at(nbReadersLaunched)->start();
                nbReadersLaunched++;
            }
            //s'il n'y a plus de lecteurs alors on lance un rédacteur,
            //s'il en reste
            else if(nbWritersLaunched < NB_THREADS_WRITER){
                //on lance le prochain rédacteur
                writerThreadsList->at(nbWritersLaunched)->start();
                nbWritersLaunched++;
            }
            //s'il ne reste plus personne alors on arrête
            else{
                continuing = false;
            }
        }

    }

    // Kill the threads

    return 0;



    // Pour ceux qui voudraient développer une version graphique
//    QApplication a(argc, argv);
//    MainWindow w;
//    w.show();

//    return a.exec();
}
