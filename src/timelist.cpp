#include "timelist.h"
#include "kersem.h"
#include "pcb.h"
#include <iostream.h>

void TimeList::add(PCB *pcb, Time time, KernelSem *kersem) {
    lock;
	time += currentTime;
		
	Elem *p = first, *prev = nullptr;
	while (p != nullptr && p->time <= time) {
		prev = p;
		p = p->next;
	}

	Elem* newelem = new Elem(pcb, time, kersem);
	((prev==nullptr)?first:prev->next) = newelem;
	newelem->next = p;
    unlock;
}

void TimeList::incUnblock() {
    lock;
	if (first != nullptr) {
        currentTime++;
        while (first != nullptr && first->time <= currentTime) {
            PCB *pcb = first->pcb;
            KernelSem *kersem = first->kersem;
            Elem *toRemove = first;
            first = first->next;
            
            //cout << "Unblocking "<< pcb->getId() << endl;

            pcb->unblock();
            pcb->unblockedTime(true);
            kersem->removeBlocked(pcb);
            delete toRemove;
        }
    } else currentTime=0;
    unlock;
}

void TimeList::remove(PCB *pcb) {
    lock;
	Elem *p = first, *prev = nullptr;
	while (p != nullptr && p->pcb != pcb) {
		prev = p;
		p = p->next;
	}

	if (p != nullptr) {
        Elem *toRemove = p;
        if (prev == nullptr) first = first->next;
        else prev->next = p->next;
        delete toRemove;
    }
    unlock;
}

TimeList::~TimeList() {
    lock;
	while (first != nullptr) {
		Elem *toRemove = first;
		first = first->next;
		delete toRemove;
	}
    unlock;
}