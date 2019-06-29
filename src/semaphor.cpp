#include "semaphor.h"
#include "kersem.h"
#include "utils.h"

Semaphore::Semaphore(int init) {
    lock;
    myImpl = new KernelSem(init);
    unlock;
}

Semaphore::~Semaphore() {
    if (myImpl != nullptr) {
        lock;
        delete myImpl;
        unlock;
        myImpl = nullptr;
    }
}

int Semaphore::wait(Time maxTimeToWait) {
    return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n) {
    return myImpl->signal(n);
}

int Semaphore::val() const {
    return myImpl->val();
}