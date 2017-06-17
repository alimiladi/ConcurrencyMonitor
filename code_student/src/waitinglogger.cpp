#include "waitinglogger.h"
#include "iostream"
#include <QThread>
#include "reader_writer_thread.h"



WaitingLogger::WaitingLogger() :
    queues(),
    mutex(QMutex::NonRecursive) {}

WaitingLogger *WaitingLogger::getInstance()
{
    static WaitingLogger *instance = new ReadWriteLogger();
    return instance;
}

WaitingQueue *WaitingLogger::contains(const QString &objectName)
{
    //Itération sur toutes les queues d'attente
    foreach (WaitingQueue* waitingQueue, queues) {
        //Si celle-ci existe
        if (waitingQueue->getName() == objectName) {
            //Nous la rnvoyons simplement
            return waitingQueue;
        }
    }
    //Sinon un pointeur nul
    return nullptr;
}

QList<WaitingQueue *> WaitingLogger::getQueues() const
{
    return queues;
}

void WaitingLogger::updateView() {}


void WaitingLogger::addWaiting(const QString &objectName)
{
    mutex.lock();
    //Récupération directe du nom du thread à partir du nom de l'objet courant
    QString threadName = QThread::currentThread()->objectName();
    //Recherche de la queue dans laquelle on est sencés rajouter
    WaitingQueue *queue = contains(objectName);
    //Si elle existe
    if(queue != nullptr){
        //On rajoute simplement du nomn du thread courant à la liste de threads en attente
        // sur cet objet
        queue->addThreadName(threadName);
    }

    //Si pas de queue
    else{
        //Création d'une nouvelle au nom de l'objet de synchro passé en argument
        queue = new WaitingQueue();
        queue->setName(objectName);
        //Rajout du nom du thread dans la nouvelle liste
        queue->addThreadName(threadName);
        queues.append(queue);
    }

    //Mise à jour de la vue
    updateView();
    mutex.unlock();
}

void WaitingLogger::removeWaiting(const QString &objectName)
{
    mutex.lock();
    //Récupération directe du nom du thread à partir du nom de l'objet courant
    QString threadName = QThread::currentThread()->objectName();

    //Recherche de la queue dans laquelle on est sencés rajouter
    WaitingQueue *queue = contains(objectName);

    //Si elle existe
    if(queue != nullptr){
        //On efface le nom du thread de la liste d'attente sinon on ne fait rien
        queue->removeThreadName(threadName);

        //Mise à jour de la vue
        updateView();
    }
    mutex.unlock();
}

QStringList ReadWriteLogger::getResourceAccesses() const
{
    return resourceAccesses;
}

ReadWriteLogger::ReadWriteLogger() {}

void ReadWriteLogger::addResourceAccess()
{
    mutex.lock();
    //Récupération directe du nom du thread à partir du nom de l'objet courant
    QString threadName = QThread::currentThread()->objectName();

    //Rajout du nom du thread dans la queue d'accès à la ressource
    resourceAccesses.append(threadName);

    //On enlève le thread de toutes les listes d'attente car il est dans la ressource
    foreach (WaitingQueue *queue, queues) {
        queue->removeThreadName(threadName);
    }

    //Mise à jour de la vue
    updateView();
    mutex.unlock();
}

void ReadWriteLogger::removeResourceAccess()
{
    mutex.lock();
    //Récupération directe du nom du thread à partir du nom de l'objet courant
    QString threadName = QThread::currentThread()->objectName();

    //On efface de la liste le nom du thread
    resourceAccesses.removeOne(threadName);

    //Mise à jour de la vue
    updateView();
    mutex.unlock();
}


void ReadWriteLogger::updateView()
{
    //Récupération du nom du thead courant à partir de l'objet currentThread
    QString name = ((ReaderWriterThread*)ReaderWriterThread::currentThread())->objectName();

    for(int i = 0; i< queues.size(); i ++){
        logs.append(queues.at(i)->getName()).append(" <- ");
        for(int j =0 ; j< queues.at(i)->getThreadNames().size() ; j ++){
            logs.append(queues.at(i)->getThreadNames().at(j));
            if(j!=queues.at(i)->getThreadNames().size()-1){
                logs.append(" - ");
            }
        }
        logs.append("\n");
    }
    logs.append("In ressource : ");
    for(int k = 0;k <resourceAccesses.size(); k++){
        logs.append(resourceAccesses.at(k));
        if(k != resourceAccesses.size()-1){
            logs.append(", ");
        }
    }

    //Affichage

    std::cout << "\n*******************logs de " << qPrintable(name) << "****************\n";
    std::cout << qPrintable(logs);
    std::cout << "\n***************************************************\n" << std::endl;

    //on efface les logs pour le thread courant
    logs.clear();
}


ReadWriteLogger *ReadWriteLogger::getInstance()
{
    //Utilitaire qui renvoie un logger casté en ReadWriteLogger
    return (ReadWriteLogger*)WaitingLogger::getInstance();
}


QString WaitingQueue::getName()
{
    return name;
}



QStringList WaitingQueue::getThreadNames()
{
    return threadNames;
}

void WaitingQueue::setName(const QString &name)
{
    this->name = name;
}

void WaitingQueue::addThreadName(const QString &threadName)
{
    //Rajout du nom du thread dans la liste inuquement si inexistant dans celleéci
    if(!threadNames.contains(threadName)){
        threadNames.append(threadName);
    }
}

void WaitingQueue::removeThreadName(const QString &threadName)
{
    threadNames.removeOne(threadName);
}

