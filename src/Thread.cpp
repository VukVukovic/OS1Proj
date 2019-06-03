#include "Thread.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include "Locker.h"
#include "Timer.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	myPCB = new PCB(stackSize, timeSlice, this);
}

Thread::~Thread() {
	//waitToComplete();
	//delete myPCB;
	//myPCB = nullptr;
}

void Thread::start() {
	myPCB->start();
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getId() {
	return myPCB->getID();
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

void dispatch() {
	disableInterrupts;
	PCB::explicitDispatch = true;
	timer();
	enableInterrupts;
}