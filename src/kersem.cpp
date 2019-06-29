#include "kersem.h"
#include "utils.h"
#include "pcb.h"
#include "SCHEDULE.H"

volatile TimeList KernelSem::blockedWaiting;

KernelSem::KernelSem(int init) {
    if (init<0) init=0;
    value = init;
}

int KernelSem::wait(Time maxTimeToWait) {
    lock;
    int ret = 1;
    if (--value < 0) {
        PCB *toBlock = (PCB*)PCB::running;
        toBlock->block();
        blocked.pushBack(toBlock);
        if (maxTimeToWait > 0)
            blockedWaiting.add(toBlock, maxTimeToWait, this);
        dispatch();
        if (toBlock->unblockedTime) {
            toBlock->unblockedTime = false;
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
    List<PCB*>::Iterator it = blocked.begin();
    if (n==0) {
        if (++value <= 0) {
            if (it.exists()) {
                (*it)->unblock();
                it.remove();
            }
        }
    } else {
        int unblocked = 0;
        value = value+n;
        while (it.exists() && n>0) {
            (*it)->unblock();
            blockedWaiting.remove(*it);
            n--;
            unblocked++;
            ++it;
        }
        ret = unblocked;
    } 
    unlock;
    return ret;
}

void KernelSem::removeBlocked(PCB *pcb) {
    List<PCB*>::Iterator it = blocked.findByValue(pcb);
    if (it.exists())
        it.remove();
}