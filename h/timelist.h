#ifndef TIMELIST_H
#define TIMELIST_H
#include "Thread.h"
#include "utils.h"
#include "list.h"

class KernelSem;

class TimeList {
public:
	struct Elem {
		PCB* pcb;
		Time time;
		Elem(PCB *pcb, Time time) : pcb(pcb), time(time) {}
	};

private:
	Time currentTime;
	List<Elem> list;

public:
	TimeList() : currentTime(0) {}
	void add(PCB *pcb, Time time);
	void tick();
	bool finished();
	bool empty();
	PCB* popFront();
	int size();
};
#endif
