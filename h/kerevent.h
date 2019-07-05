#ifndef KEREVENT_H
#define KEREVENT_H
#include "semaphor.h"
#include "event.h"

class PCB;

class KernelEv {
    IVTNo ivtNo;
    Semaphore sem;
    PCB* pcb;
public:
    KernelEv(IVTNo ivtNo);
    ~KernelEv();
    void wait();
    void signal();
};
#endif