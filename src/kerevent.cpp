#include "kerevent.h"
#include "utils.h"
#include "ivtentry.h"
#include "pcb.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), pcb((PCB*)PCB::running), sem(0) {
    IVTEntry::ivtable[ivtNo]->setEvent(this);
}
KernelEv::~KernelEv() {
    IVTEntry::ivtable[ivtNo]->clearEvent();
}

void KernelEv::wait() { 
    if (pcb == PCB::running) 
        sem.wait(0);
}

void KernelEv::signal() {
    if (sem.val()<0)
        sem.signal(); 
}