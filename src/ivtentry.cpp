#include "ivtentry.h"
#include "kerevent.h"
#include "utils.h"
#include <dos.h>

IVTEntry* IVTEntry::ivtable[ENTRY_NUM] = {nullptr};

IVTEntry::IVTEntry(IVTNo num, pInterrupt newRoutine) : num(num), newRoutine(newRoutine), event(nullptr)  {
    ivtable[num] = this;
}

IVTEntry::~IVTEntry() {
    clearEvent();
}

void IVTEntry::setEvent(KernelEv *event) {
    this->event = event;
    disableInterrupts;
    oldRoutine = getvect(num);
    setvect(num, newRoutine);
    enableInterrupts;
}

void IVTEntry::clearEvent() {
    disableInterrupts;
    setvect(num, oldRoutine);
    enableInterrupts;
    event = nullptr;
    ivtable[num] = nullptr;
}

void IVTEntry::signal() {
    event->signal();
}