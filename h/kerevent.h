#ifndef KEREVENT_H
#define KEREVENT_H
#include "semaphor.h"
#include "event.h"
#include "pcb.h"

class KernelEv {
    IVTNo ivtNo;
    Semaphore sem;
    PCB* pcb;
public:
    KernelEv(IVTNo ivtNo);
    ~KernelEv();
    void wait() { if (pcb == PCB::running) sem.wait(0); }
    void signal() { if (sem.val()<0) sem.signal(); }
};
#endif