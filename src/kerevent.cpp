#include "kerevent.h"
#include "utils.h"
#include "ivtentry.h"

KernelEv::KernelEv(IVTNo ivtNo) : ivtNo(ivtNo), pcb((PCB*)PCB::running), sem(0) {
    IVTEntry::ivtable[ivtNo]->setEvent(this);
}
KernelEv::~KernelEv() {
    IVTEntry::ivtable[ivtNo]->restore();
}