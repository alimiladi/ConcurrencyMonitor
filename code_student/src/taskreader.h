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
    TaskReader(const unsigned int &id, const QString &name, AbstractReaderWriter *resource, SynchroController* synchroController);
    TaskReader();


protected:

    void run() Q_DECL_OVERRIDE;

private:

    unsigned int id;
    AbstractReaderWriter *resource;
    SynchroController* synchroController;


};

#endif // TASKREADER_H
