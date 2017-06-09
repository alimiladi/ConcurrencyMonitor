#ifndef READER_WRITER_THREAD_H
#define READER_WRITER_THREAD_H


#include <iostream>
#include <QThread>
#include <QString>
#include "abstractreaderwriter.h"
#include "synchrocontroller.h"
#include "waitinglogger.h"

class ReaderWriterThread : public QThread{

    Q_OBJECT
public:
    unsigned int getId(){
        return id;
    }

    unsigned int getFirstTime(){
        return firstTime;
    }

    void setFirstTime(bool value){
        this->firstTime = value;
    }


protected:

    ReaderWriterThread();

    void run() Q_DECL_OVERRIDE;

    //identifiant unique pour les logs
    unsigned int id;

    bool firstTime = true;

    AbstractReaderWriter *resource;

    SynchroController* synchroController;

    ReadWriteLogger* waitingLogger;
};

class TaskReader : public ReaderWriterThread{

    Q_OBJECT

public:
    TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource);
    TaskReader();

protected:
    void run() Q_DECL_OVERRIDE;

};

class TaskWriter : public ReaderWriterThread{

    Q_OBJECT

public:
    TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource);
    TaskWriter();

protected:
    void run() Q_DECL_OVERRIDE;
};


#endif // READER_WRITER_THREAD_H
