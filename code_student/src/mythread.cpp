#include "mythread.h"


MyThread::MyThread(const QString &threadName){

    QThread::setObjectName(threadName);
}

void MyThread::run(){
}
