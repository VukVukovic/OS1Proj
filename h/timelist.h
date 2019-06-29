#ifndef TIMELIST_H
#define TIMELIST_H
#include "Thread.h"
#include "utils.h"

class KernelSem;

class TimeList {
	struct Elem {
		PCB* pcb;
		Time time;
        KernelSem *kersem;
		Elem* next;
		Elem(PCB *pcb, Time time, KernelSem *kersem) : pcb(pcb), time(time),kersem(kersem),next(nullptr) {}
	};

	Time currentTime;
	Elem *first;

public:
	TimeList() : currentTime(0), first(nullptr) {}
	void add(PCB *pcb, Time time, KernelSem *kersem);
	void incUnblock();
	void remove(PCB *pcb);
	~TimeList();
};
#endif
