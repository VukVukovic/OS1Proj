#include "Thread.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "utils.h"
#include "timer.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread() {
	delete myPCB;
	myPCB = nullptr;
}

void Thread::start() {
	lock;
	myPCB->state = READY;
	Scheduler::put(myPCB);
	unlock;
}

void Thread::waitToComplete() {
	if (myPCB==nullptr) return; // EXCEPTION!
	myPCB->waitToComplete();
}

void dispatch() {
    disableInterrupts;
	int lockSave = lockCnt;
	lockCnt = 0;
	PCB::timerCall = true;
	timer();
	lockCnt = lockSave;
	enableInterrupts;
}