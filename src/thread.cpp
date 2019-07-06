#include "Thread.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "utils.h"
#include "timer.h"

Thread::Thread (StackSize stackSize, Time timeSlice) {
	lock;
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock;
}

Thread::~Thread() {
	waitToComplete();
	lock;
	if (myPCB!=nullptr)
		delete myPCB;
	unlock;
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
	explicitCall = true;
	timer();
	enableInterrupts;
}

void Thread::signal(SignalId signal) { 
	if (myPCB==nullptr) return;
	myPCB->signal(signal); 
}
void Thread::registerHandler(SignalId signal, SignalHandler handler) { 
	if (myPCB==nullptr) return;
	myPCB->registerHandler(signal,handler); 
}
void Thread::unregisterAllHandlers(SignalId id) { 
	if (myPCB==nullptr) return;
	myPCB->unregisterAllHandlers(id); 
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) { 
	if (myPCB==nullptr) return;
	myPCB->swap(id, hand1, hand2); 
}

void Thread::blockSignal(SignalId signal) {
	if (myPCB==nullptr) return;
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal) { PCB::blockSignalGlobally(signal); }
void Thread::unblockSignal(SignalId signal) { 
	if (myPCB==nullptr) return;
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal) { PCB::unblockSignalGlobally(signal); }