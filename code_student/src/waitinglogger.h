/** @file waitinglogger.h
 *  @brief enregistre l'état d'une ressource et de ses files
 *
 *  @author Ali Miladi, Quentin Zeller, Julien Brêchet et Adrien Marco
 *  @date 15.06.2017
 *  @bug No known bug
 *
 * Nous avons ici deux classes, une représentant un logger de manière générale
 * et une autre plus adaptée au contexte des logs pour un cas de lecteurs/rédacteurs
 * Le but est de stocker les logs et de recevoir les nouvelles mises à jour.
 */


#ifndef WAITINGLOGGER_H
#define WAITINGLOGGER_H

#include <QStringList>
#include <QList>
#include <QMutex>

/**
 * @brief The WaitingQueue class
 * représente un file d'attente caractérisée par
 * son nom et la liste des threads qui attendent dessus
 */
class WaitingQueue
{
    //nom de la file
    QString name;
    //liste des threads (noms)
    QStringList threadNames;


public:

    /**
     * @brief getName
     * @return le nom de la file
     */
    QString getName();

    /**
     * @brief getThreadNames
     * @return la liste des threads de la file
     */
    QStringList getThreadNames();

    /**
     * @brief setName
     * @param name: nom de la file
     * setter pour le nom de la file
     */
    void setName(const QString &name);

    /**
     * @brief addThreadName
     * @param threadName: nom du thread à ajouter à la file
     * Pour ajouter un thread à une file d'attente
     */
    void addThreadName(const QString &threadName);

    /**
     * @brief removeThreadName
     * @param threadName: nom du thread à supprimer de la file
     * Pour supprimer un thread à une file d'attente
     */
    void removeThreadName(const QString &threadName);
};





/**
 * @brief The WaitingLogger class => singleton
 * représente le logger de base dont va hériter un logger
 * plus spécifique comme le ReadWriteLogger.
 */
class WaitingLogger
{
public:

    /**
     * @brief getInstance Returns the singleton
     * @return the only instance of WaitingLogger
     * This logger has to be a singleton, accessed by every synchronization
     * object
     */
    static WaitingLogger *getInstance();

    /**
     * @brief addWaiting
     * @param objectName: nom du thread
     * Pour ajouter un thread à une file d'attente
     */
    void addWaiting(const QString& objectName);

    /**
     * @brief removeWaiting
     * @param objectName: nom du thread
     * Pour enlever un thread d'une file d'attente
     */
    void removeWaiting(const QString& objectName);


    /**
     * @brief WaitingLogger::contains
     * Pour voir si la liste existe déjà dans le logger
     * @param objectName
     * @return
     */
    WaitingQueue *contains(const QString &objectName);

protected:

    /**
     * @brief WaitingLogger
     * constructeur de la classe
     */
    WaitingLogger();


    /**
     * @brief getQueues: getter
     * @return la liste des files d'attente
     */
    QList<WaitingQueue *> getQueues() const;


    /**
     * @brief updateView Responsible for updating the view
     * This function should do what is required to update the view.
     * In this first version it could simply modify the display in
     * the terminal.
     */
    virtual void updateView();

    //liste des files d'attentes
    QList<WaitingQueue *> queues;

    //mutex pour protéger les mises à jour des logs
    QMutex mutex;

    //contient les logs à afficher
    QString logs;

};




/**
 * @brief The ReadWriteLogger class
 * logger spécifique au contexte lecteur/rédacteur
 * => singleton
 */
class ReadWriteLogger : public WaitingLogger
{
    friend WaitingLogger;

public:

    /**
     * @brief getResourceAccesses
     * @return la liste des threads qui sont dans la ressource
     */
    QStringList getResourceAccesses() const;

    /**
     * @brief addResourceAccess
     * ajoute un thread à la ressource
     */
    void addResourceAccess();

    /**
     * @brief removeResourceAccess
     * enlève un thread de la ressource
     */
    void removeResourceAccess();

    /**
     * @brief getInstance
     * @return l'unique instance du logger ReadWriteLogger
     */
    static ReadWriteLogger *getInstance();


protected:

    /**
     * @brief ReadWriteLogger
     * constructeur de la classe
     */
    ReadWriteLogger();

    /**
     * @brief updateView Responsible for updating the view
     * This function should do what is required to update the view.
     * In this first version it could simply modify the display in
     * the terminal.
     */
    virtual void updateView();

    //liste des threads ayant accès à la ressource
    QStringList resourceAccesses;

};

#endif // WAITINGLOGGER_H
