#include "reader_writer_thread.h"
#include "stdlib.h"
#include "waitinglogger.h"

ReaderWriterThread::ReaderWriterThread()
{
    synchroController = SynchroController::getInstance();
    waitingLogger = ReadWriteLogger::getInstance();
}


void ReaderWriterThread::run(){}


TaskReader::TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource)
{
    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}


void TaskReader::run()
{

    while(1) {
        //Comme énnoncé dans la consigne du laboratoire, un thread fait une pause pour afficher les
        // logs des objets de synchronisation qu'il utilise avant chaque accès à la ressource et à
        // la sortie de celle-ci
        synchroController->pause();
        resource->lockReading();

        //Rajout du nom du thread dans la liste d'accès à la ressource
        waitingLogger->addResourceAccess();

        synchroController->pause();
        //Le thread s'occupe d'éffacer son nom de la liste à la sortie dde la ressource
        waitingLogger->removeResourceAccess();

        resource->unlockReading();
    }
}


TaskWriter::TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource)
{
    this->id = id;
    QThread::setObjectName(name);
    this->resource = resource;
}


void TaskWriter::run()
{

    while(1) {
        //Comme énnoncé dans la consigne du laboratoire, un thread fait une pause pour afficher les
        // logs des objets de synchronisation qu'il utilise avant chaque accès à la ressource et à
        // la sortie de celle-ci
        synchroController->pause();
        resource->lockWriting();

        //Rajout du nom du thread dans la liste d'accès à la ressource
        waitingLogger->addResourceAccess();

        synchroController->pause();
        //Le thread s'occupe d'éffacer son nom de la liste à la sortie dde la ressource
        waitingLogger->removeResourceAccess();

        resource->unlockWriting();
    }
}
