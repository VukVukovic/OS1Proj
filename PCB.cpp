#include "PCB.h"
#include "SCHEDULE.h"
#include "Util.h"
#include "SlpList.h"

volatile PCB* PCB::running = nullptr;
ID PCB::ID0 = 0;
SleepList PCB::sleepList;

PCB::PCB() {
	sp = ss = bp = 0;
	finished = false;
	kvant=1;
	stack = nullptr;
	id = ++ID0;
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	this->myThread = myThread;

	stack = new unsigned[stackSize];
	stack[stackSize-1] = 0x200;
	stack[stackSize-2] = FP_SEG(PCB::runner);
	stack[stackSize-3] = FP_OFF(PCB::runner);

	sp = FP_OFF(stack+stackSize-STACK_REG_OFFSET);
	ss = FP_SEG(stack+stackSize-STACK_REG_OFFSET);
	bp = sp;
	finished = false;
	sleeping = false;
	kvant = timeSlice;
	id = ++ID0;
}

void PCB::runner() {
	PCB::running->myThread->run();
	running->finished = true;
	dispatch();
}

void PCB::sleep(Time timeToSleep) {
	PCB::running->sleeping=true;
	sleepList.add((PCB*)PCB::running, timeToSleep);
	dispatch();
}

void PCB::decSleepingWake() {
	sleepList.decFirst();
	while (sleepList.finishedSleep()) {
		PCB *wokenup = PCB::sleepList.get();
		wokenup->sleeping = false;
		Scheduler::put(wokenup);
	}
}

