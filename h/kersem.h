#ifndef KERSEM_H
#define KERSEM_H
#include "Thread.h"
#include "list.h"
#include "timelist.h"

class KernelSem {
    int value;
    List<PCB*> blocked;
    TimeList blockedWaiting;
    void tickCheck();

public:
    static List<KernelSem*> semaphores;
    KernelSem(int init);
    int wait(Time maxTimeToWait);
    int signal(int n);
    int val() const { return value; }
    static void tick();
    ~KernelSem();
};
#endif