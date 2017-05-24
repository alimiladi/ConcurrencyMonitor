#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <iostream>
#include <QThread>
#include <QString>

class MyThread : public QThread{

    Q_OBJECT

public:

    MyThread(const QString &threadName);

protected:


    void run() Q_DECL_OVERRIDE;

private:


};

#endif // MYTHREAD_H
