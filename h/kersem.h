#ifndef KERSEM_H
#define KERSEM_H
#include "Thread.h"
#include "list.h"
#include "timelist.h"

class KernelSem {
    int value;
    List<PCB*> blocked;
public:
    volatile static TimeList blockedWaiting;
    KernelSem(int init);
    int wait(Time maxTimeToWait);
    int signal(int n);
    int val() const { return value; }
    void removeBlocked(PCB *pcb);
};
#endif