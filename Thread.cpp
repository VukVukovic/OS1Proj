#include "Thread.h"
#include "PCB.h"
#include "SCHEDULE.H"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}
Thread::~Thread() {}

void Thread::start() {
	//lock
	Scheduler::put(myPCB);
	//unlock
}
void Thread::waitToComplete() {}
void Thread::sleep(Time timeToSleep) {}
