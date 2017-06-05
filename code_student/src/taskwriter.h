#ifndef TASKWRITER_H
#define TASKWRITER_H


#include <iostream>
#include <QThread>
#include <QString>
#include "abstractreaderwriter.h"

class TaskWriter : public QThread{

    Q_OBJECT

public:
    TaskWriter(const unsigned int &id, const QString &name, AbstractReaderWriter *resource);


protected:

    void run() Q_DECL_OVERRIDE;

private:

    unsigned int id;
    AbstractReaderWriter *resource;


};

#endif // TASKWRITER_H
