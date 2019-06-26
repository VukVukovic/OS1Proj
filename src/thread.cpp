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
	if (myPCB==nullptr) return;
	myPCB->start();
}

ID Thread::getId() {
	if (myPCB==nullptr) return -1;
	return myPCB->getId();
}

ID Thread::getRunningId() {
	return PCB::getRunningId();
}

Thread* Thread::getThreadById(ID id) {
	return PCB::getThreadById(id);
}

void Thread::waitToComplete() {
	if (myPCB==nullptr) return;
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