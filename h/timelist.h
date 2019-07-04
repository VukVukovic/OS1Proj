#ifndef TIMELIST_H
#define TIMELIST_H
#include "Thread.h"
#include "utils.h"
#include "list.h"

class KernelSem;

class TimeList {
	struct Elem {
		PCB* pcb;
		Time time;
        KernelSem *kersem;
		Elem(PCB *pcb, Time time, KernelSem *kersem) : pcb(pcb), time(time), kersem(kersem) {}
	};


	Time currentTime;
	List<Elem> list;

public:
	TimeList() : currentTime(0) {}
	void add(PCB *pcb, Time time, KernelSem *kersem);
	void incUnblock();
	void remove(PCB *pcb);
};
#endif
