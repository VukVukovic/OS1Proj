#include "Thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Locker.h"
#include "Timer.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread() {
	waitToComplete();
}

void Thread::start() {
	lock;
	myPCB->state = READY;
	Scheduler::put(myPCB);
	unlock;
}
void Thread::waitToComplete() {}

ID Thread::getId() {
	return myPCB->getID();
}

ID Thread::getRunningId() {
	return ((PCB*)PCB::running)->getID();
}

void dispatch() {
	disableInterrupts;
	PCB::explicitDispatch = true;
	timer();
	enableInterrupts;
}