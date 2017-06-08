#ifndef TASKREADER_H
#define TASKREADER_H


#include <iostream>
#include <QThread>
#include <QString>
#include "abstractreaderwriter.h"
#include "synchrocontroller.h"

class TaskReader : public QThread{

    Q_OBJECT

public:
    TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource);
    TaskReader();


protected:

    void run() Q_DECL_OVERRIDE;

private:

    //identifiant unique pour les logs
    unsigned int id;
    AbstractReaderWriter *resource;


};

#endif // TASKREADER_H
