#include "osemaphore.h"

OSemaphore::OSemaphore(int n):
    sem(n){}

void OSemaphore::acquire(){
    sem.acquire();
}

void OSemaphore::release(){
    sem.release();
}

bool OSemaphore::tryAcquire(){
    sem.tryAcquire();
}
