#include "mythread.h"


MyThread::MyThread(const QString &threadName){

    this->threadName = threadName;
}

void MyThread::run(){


    std::cout << threadName.toStdString() << std::endl;
}
