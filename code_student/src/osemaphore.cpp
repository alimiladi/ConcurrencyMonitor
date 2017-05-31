#include "osemaphore.h"

OSemaphore::OSemaphore(int n)
{
    sem(n);
}

OSemaphore::~OSemaphore(){}

OSemaphore::acquire(){
    sem.acquire();
}

OSemaphore::release(){
    sem.release();
}

OSemaphore::tryAcquire(){
    sem.tryAcquire();
}
