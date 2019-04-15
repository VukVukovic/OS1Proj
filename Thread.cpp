#include "Thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Locker.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}
Thread::~Thread() {}

void Thread::start() {
	Locker::lock();
	Scheduler::put(myPCB);
	Locker::unlock();
}
void Thread::waitToComplete() {}

/*
void Thread::sleep(Time timeToSleep) {
	Locker::lock();
	PCB::sleep(timeToSleep);
	Locker::unlock();
} */

void dispatch(){
	Locker::lock();
	PCB::explicitDispatch = true;
	timer();
	Locker::unlock();
}
