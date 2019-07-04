#include "kersem.h"
#include "utils.h"
#include "pcb.h"

List<KernelSem*> KernelSem::semaphores{};

KernelSem::KernelSem(int init) {
    if (init<0) init=0;
    value = init;
    lock;
    cout << "Semaphore creation" << endl;
    semaphores.pushBack(this);
    unlock;
}

int KernelSem::wait(Time maxTimeToWait) {
    lock;
    int ret = 1;
    if (--value < 0) {
        PCB *toBlock = (PCB*)PCB::running;
        toBlock->block();

        if (maxTimeToWait==0)
            blocked.pushBack(toBlock);
        else
            blockedWaiting.add(toBlock, maxTimeToWait);
        
        dispatch();

        if (toBlock->unblockedTime()) {
            toBlock->unblockedTime(false);
            ret = 0;
        }
    }
    unlock;
    return ret;
}

int KernelSem::signal(int n) {
    if (n<0) return n;

    int ret = 0;
    lock;
    int maxUnblock = (n==0)?1:n;
    value += maxUnblock;

    while (!blocked.empty() && maxUnblock>0) {
        PCB* toUnblock = blocked.popFront();
        toUnblock->unblock();
        maxUnblock--;
        ret++;
    }

    while (!blockedWaiting.empty() && maxUnblock>0) {
        PCB* toUnblock = blockedWaiting.popFront();
        toUnblock->unblock();
        maxUnblock--;
        ret++;
    }
    if (n==0) ret=0;
    unlock;
    return ret;
}

KernelSem::~KernelSem() {
    lock;
    List<KernelSem*>::Iterator it = semaphores.begin();
    while (it.exists() && (*it) != this) ++it;
    if (it.exists()) it.remove();
    unlock;
}

void KernelSem::tick() {
    for (List<KernelSem*>::Iterator it = semaphores.begin();it.exists();++it)
        (*it)->tickCheck();
}

void KernelSem::tickCheck() {
    blockedWaiting.tick();
    while (blockedWaiting.finished()) {
        PCB* toUnblock = blockedWaiting.popFront();
        toUnblock->unblock();
        toUnblock->unblockedTime(true);
    }
}