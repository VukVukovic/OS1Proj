#include "kersem.h"
#include "utils.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include <iostream.h>
#include <assert.h>

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
    if (n==0) {
        if (++value <= 0 && blocked.size()>0) {
            PCB* toUnblock = blocked.popFront();
            toUnblock->unblock();
            blockedWaiting.remove(toUnblock);
        }
    } else {
        int unblocked = 0;
        value = value+n;
        List<PCB*>::Iterator it = blocked.begin();
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
    for (List<PCB*>::Iterator it = blocked.begin(); it.exists(); ++it) {
        if (*it == pcb) { 
            it.remove();
            break; 
        }
    }
}