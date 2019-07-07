#include "pcb.h"
#include "SCHEDULE.h"
#include "utils.h"
#include <dos.h>
#include <iostream.h>

volatile PCB* PCB::running = nullptr;
volatile List<PCB*> PCB::allPCBs;
volatile ID PCB::ID0 = 0;
volatile SignalStatus PCB::globalPermission(~0U);

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)(), State s) 
: localPermission(PCB::running->localPermission)
{
	if (stackSize<minStackSize) stackSize = minStackSize;
	if (stackSize>maxStackSize) stackSize = maxStackSize;

	unsigned long stackCount = stackSize/sizeof(unsigned);
	lock;
	stack = new unsigned[stackCount];
	unlock;
	stack[stackCount-1] = 0x200; // PSWI=1
	stack[stackCount-2] = FP_SEG(fun);
	stack[stackCount-3] = FP_OFF(fun);

	sp = FP_OFF(stack+stackCount-12);
	ss = FP_SEG(stack+stackCount-12);
	bp = sp;
	lockCount = 0;
	state = s;
	this->timeSlice = ((timeSlice==0)?-1:timeSlice);
	this->myThread = myThread;
	unblTime = false;

	parent = PCB::running->getId();
    lock;
	id = ++ID0;
	if (state!=IDLE) {
		for (int i=0;i<16;i++) handlers[i] = PCB::running->handlers[i];
		allPCBs.pushBack(this);
	} else
		localPermission.clearAll();	// no signals for idle
    unlock;
}

PCB::PCB() : localPermission(~0U) { // Kernel thread
    stack = nullptr;
    sp = ss = bp = 0;
	lockCount = 0;
	state = RUNNING;
	timeSlice = defaultTimeSlice;
	myThread = nullptr;
	unblTime = false;
	parent = 0;
	
	lock;
	id = ++ID0;
	allPCBs.pushBack(this);
    unlock;
}

PCB::~PCB() {
	lock;
	if (stack != nullptr) { // Kernel thread does not have allocated stack
		delete[] stack;
		stack = nullptr;
	}
	// Remove pcb from global pcb list
	List<PCB*>::Iterator it = allPCBs.begin();
	while (it.exists() && *it != this) ++it;
	if (it.exists()) 
		it.remove();
	unlock;
}

void PCB::start() {
	lock;
	if (state == CREATED) {
		state = READY;
		Scheduler::put(this);
	}
	unlock;
}

void PCB::runner() {
	running->myThread->run();
	lock;
	running->releaseWaiting();

	// System signals
	Thread *parentThr = Thread::getThreadById(running->parent);
	if (parentThr!=nullptr && parentThr->myPCB!=nullptr) parentThr->myPCB->signal(1);
	running->signal(2); 
	// handle signal 2 and other if left unhandled
	running->handleSignals();

	running->state = FINISHED;
	dispatch();
	unlock;
}

void idleMethod() {
	//cout << "Started idle!" << endl;
	while(true);
}

PCB* PCB::getIdlePCB() {
	static PCB idlePCB(minStackSize,1,nullptr,idleMethod,IDLE);
	return &idlePCB;
}

void PCB::waitToComplete() {
	lock;
	if (PCB::running != this && state != FINISHED && state != CREATED && state != IDLE) {
		PCB::running->block();
		waiting.pushFront((PCB*)PCB::running);
		dispatch();
	}
	unlock;
}

void PCB::releaseWaiting() {
	lock;
	while (!waiting.empty()) {
		PCB *releasePCB = waiting.popFront();
		releasePCB->unblock();
	}
	unlock;
}

ID PCB::getRunningId() {
	return PCB::running->id;
}

Thread* PCB::getThreadById(ID id) {
	Thread *thr = nullptr;
	lock;
	for (List<PCB*>::Iterator it = allPCBs.begin(); it.exists(); ++it)
		if ((*it)->getId() == id){
			thr = (*it)->myThread;
			break;
		}
	unlock;
	return thr;
}

void PCB::unblock() {
	lock;
	state = READY;
	Scheduler::put(this);
	unlock;
}

void PCB::block() {
	lock;
	state = BLOCKED;
	unlock;
}

void PCB::signal(SignalId signal) {
	if (signal>=16) return;
	lock;
	activeSignals.pushBack(signal);
	unlock;
}
void PCB::registerHandler(SignalId signal, SignalHandler handler) {
	lock;
	if (signal < 16)
		handlers[signal].pushBack(handler);
	unlock;
}
void PCB::unregisterAllHandlers(SignalId id) {
	lock;
	if (id < 16)
		handlers[id].clear(); 
	unlock;
}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	if (id >= 16) return;
	lock;
	List<SignalHandler>::Iterator it1, it2;
	for (List<SignalHandler>::Iterator it = handlers[id].begin(); it.exists(); ++it) {
		if (!it1.exists() && (*it1) == hand1) it1 = it;
		if (!it2.exists() && (*it2) == hand2) it2 = it;
		if (it1.exists() && it2.exists()) break;
	}
	List<SignalHandler>::swap(it1, it2);
	unlock;
}

void PCB::blockSignal(SignalId signal) { lock; localPermission.reset(signal); unlock; }
void PCB::blockSignalGlobally(SignalId signal) { lock; globalPermission.reset(signal); unlock; }
void PCB::unblockSignal(SignalId signal) { lock; localPermission.set(signal); unlock; }
void PCB::unblockSignalGlobally(SignalId signal) { lock; globalPermission.set(signal); unlock; }

bool PCB::handleSignals() {
	PCB *pcb = (PCB*)running;
	for (List<SignalId>::Iterator it = pcb->activeSignals.begin(); it.exists(); ++it) {
		SignalId id = *it;
		if (pcb->localPermission.status(id) && globalPermission.status(id)) {
			if (id == 0) 
				return true; // kill thread
			List<SignalHandler>::Iterator handit = (pcb->handlers)[id].begin();
			while (handit.exists()) {
				(*handit)();
				++handit;
			}
			it.remove();
		}
	}
	return false;
}

void PCB::killThread(PCB *toDelete) {
	toDelete->myThread->myPCB=nullptr;
	toDelete->releaseWaiting();
	delete toDelete;
}