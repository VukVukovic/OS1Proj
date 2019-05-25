#include "Thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Locker.h"
#include "Timer.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread() {}

void Thread::start() {
	Locker::lock();
	myPCB->state = READY;
	Scheduler::put(myPCB);
	Locker::unlock();
}
void Thread::waitToComplete() {}

ID Thread::getId() {
	return myPCB->getID();
}

ID Thread::getRunningId() {
	return PCB::running->getID();
}

void dispatch(){
	asm cli; // CHECK THIS
	PCB::explicitDispatch = true;
	timer();
	asm sti;
}