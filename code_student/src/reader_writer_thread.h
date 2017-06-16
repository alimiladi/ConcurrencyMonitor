/** @file reader_writer_thread.h
 *  @brief threads lecteurs et rédacteurs
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug pas de bugs connus
 *
 * Il s'agit ici de représenter une classe lecteurs rédacteurs (ReaderWriterThread)
 * dont les classes TaskReader et TaskWriter vont hériter. Ces deux classes
 * représentent respectivement un thread lecteur et un rédacteur.
 * Pour cela on fait hériter ReaderWriterThread de QThread et on utilise
 * l'héritage pour récupérer l'identifiant du thread courant. Cela est en
 * effet nécessaire car il s'agit d'un identifiant que nous settons nous même
 * et non celui que fournit la classe QThread. Cet identifiant est seulement utilisé
 * lors de l'affichage des logs à titre d'information supplémentaire et donc pas des plus
 * nécessaire. Si cet identifiant venait à être enlevé, ce système d'héritage ne
 * serait plus nécessaire. Cependant, nous avons profité pour factoriser certains
 * éléments.
 */

#ifndef READER_WRITER_THREAD_H
#define READER_WRITER_THREAD_H


#include <iostream>
#include <QThread>
#include <QString>
#include "abstractreaderwriter.h"
#include "synchrocontroller.h"
#include "waitinglogger.h"

/**
 * @brief The ReaderWriterThread class
 * Représente un thread lecteur ou redacteur
 * Hérite simplement de QThread pour représenter un thread
 * Est seulement là pour fournir un getteur sur l'id du thread courant
 */
class ReaderWriterThread : public QThread{

    Q_OBJECT
public:

    /**
     * @brief getId
     * @return l'identifiant du thread
     */
    unsigned int getId(){
        return id;
    }



protected:

    /**
     * @brief ReaderWriterThread
     * constructeur de la classe
     */
    ReaderWriterThread();

    /**
     * @brief run
     * méthode run vide dont les lecteurs et rédacteurs fourniront
     * une implémentation
     */
    void run() Q_DECL_OVERRIDE;

    //identifiant unique pour les logs
    unsigned int id;

    //ressource abstraite dont les threads voudront accéder
    AbstractReaderWriter *resource;

    //synchroniseur du système pour les pauses et départs
    SynchroController* synchroController;

    //contrôleur des logs, contient l'état des files
    ReadWriteLogger* waitingLogger;
};





/**
 * @brief The TaskReader class
 * représente un thread lecteur
 */
class TaskReader : public ReaderWriterThread{

    Q_OBJECT

public:

    /**
     * @brief TaskReader
     * @param id : id du thread lecteur
     * @param name: nom du thread lecteur
     * @param resource: ressource que le thread lecteur voudra accéder
     * constructeur de la classe
     */
    TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource);


protected:

    /**
     * @brief run
     * décrit le fonctionnement du lecteur et les actions qu'il effectue
     */
    void run() Q_DECL_OVERRIDE;

};



/**
 * @brief The TaskWriter class
 * représente un thread rédacteur
 */
class TaskWriter : public ReaderWriterThread{

    Q_OBJECT

public:

    /**
     * @param id : id du thread rédacteur
     * @param name: nom du thread rédacteur
     * @param resource: ressource que le thread rédacteur voudra accéder
     * constructeur de la classe
     */
    TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource);

protected:

    /**
     * @brief run
     * décrit le fonctionnement du rédacteur et les actions qu'il effectue
     */
    void run() Q_DECL_OVERRIDE;
};


#endif // READER_WRITER_THREAD_H
