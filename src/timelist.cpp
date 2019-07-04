#include "timelist.h"
#include "kersem.h"
#include "pcb.h"
#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include "SCHEDULE.H"

void TimeList::add(PCB *pcb, Time time) {
    lock;
	time += currentTime;
    List<Elem>::Iterator it = list.begin();
    while (it.exists() && (*it).time <= time) ++it;
    it.insertBefore(Elem(pcb, time));
    unlock;
}

void TimeList::tick() {
    if (!list.empty()) currentTime++;
    else currentTime=0;
}

bool TimeList::finished() {
    if (list.empty()) return false;
    return (*(list.begin())).time <= currentTime;
}

bool TimeList::empty() {
    return list.empty();
}

PCB* TimeList::popFront() {
    return (list.popFront()).pcb;
}