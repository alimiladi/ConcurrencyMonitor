#ifndef WAITINGLOGGER_H
#define WAITINGLOGGER_H

#include <QStringList>
#include <QList>
#include <QMutex>

class WaitingQueue
{
    QString name;
    QStringList threadNames;    

public:
    QString getName();

    QStringList getThreadNames();

    void setName(const QString &name);

    void addThreadName(const QString &threadName);

    void removeThreadName(const QString &threadName);
};

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

    //affiche les logs générés par un thread
    void printLogs(unsigned int id);

    //set la taille du tableau des logs, un emplacement pour chaque thread
    void setSizeLogs(unsigned int nbThreads);

    //pour vider les logs d'un thread => appelé lors du resume du synchrocontroller
    void clearLogs(unsigned int id);

    void addWaiting(const QString& objectName, unsigned int id);

    void removeWaiting(const QString& objectName, unsigned int id);


    /**
     * @brief WaitingLogger::contains
     * Pour voir si la liste existe déjà dans le logger
     * @param objectName
     * @return
     */
    WaitingQueue *contains(const QString &objectName);

protected:
    WaitingLogger();

    QList<WaitingQueue *> getQueues() const;

    /**
     * @brief updateView Responsible for updating the view
     * This function should do what is required to update the view.
     * In this first version it could simply modify the display in
     * the terminal.
     */
    virtual void updateView(unsigned int id);

    QList<WaitingQueue *> queues;
    QMutex mutex;

    //contient les logs à afficher
    QList<QString*>* logs = new QList<QString*>();


};

class ReadWriteLogger : public WaitingLogger
{
    friend WaitingLogger;

public:

    QStringList getResourceAccesses() const;

    void addResourceAccess(unsigned int id);
    void removeResourceAccess(unsigned int id);

    static ReadWriteLogger *getInstance();

    void print(QString string);

protected:

    ReadWriteLogger();

    /**
     * @brief updateView Responsible for updating the view
     * This function should do what is required to update the view.
     * In this first version it could simply modify the display in
     * the terminal.
     */
    //met a jour les logs de un thread
    virtual void updateView(unsigned int id);

    QStringList resourceAccesses;

};

#endif // WAITINGLOGGER_H
