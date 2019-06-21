#include "Thread.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "utils.h"
#include "timer.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread() {}

void Thread::start() {
	lock;
	myPCB->state = READY;
	Scheduler::put(myPCB);
	unlock;
}

void dispatch() {
    disableInterrupts;
	PCB::timerCall = true;
	timer();
	enableInterrupts;
}