#include "timelist.h"
#include "kersem.h"
#include "pcb.h"
#include <iostream.h>
#include <assert.h>

void TimeList::add(PCB *pcb, Time time, KernelSem *kersem) {
    lock;
	time += currentTime;
    List<Elem>::Iterator it = list.begin();
    while (it.exists() && (*it).time <= time) ++it;
    it.insertBefore(Elem(pcb, time, kersem));
    unlock;
}

void TimeList::incUnblock() {
    lock;
	if (!list.empty()) {
        currentTime++;
        List<Elem>::Iterator it = list.begin();
        while (it.exists() && (*it).time <= currentTime) {
            cout << "UNBLOCKING!" << endl;
            PCB *pcb = (*it).pcb;
            KernelSem *kersem = (*it).kersem;

            pcb->unblock();
            pcb->unblockedTime(true);
            kersem->removeBlocked(pcb);
            it.remove();
            ++it;
        }
    } else currentTime=0;
    unlock;
}

void TimeList::remove(PCB *pcb) {
    lock;
    List<Elem>::Iterator it = list.begin();
    while (it.exists() && (*it).pcb != pcb) ++it;
    if (it.exists())
        it.remove();
    unlock;
}