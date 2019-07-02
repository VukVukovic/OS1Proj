#include "pcb.h"
#include "SCHEDULE.h"
#include "utils.h"
#include <dos.h>
#include <iostream.h>

volatile PCB* PCB::running = nullptr;
volatile List<PCB*> PCB::allPCBs;
volatile ID PCB::ID0 = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)(), State s) {
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

    lock;
	id = ++ID0;
	if (state!=IDLE)
		allPCBs.pushBack(this);
    unlock;
}

PCB::PCB() { // Kernel thread
    stack = nullptr;
    sp = ss = bp = 0;
	lockCount = 0;
	state = RUNNING;
	timeSlice = defaultTimeSlice;
	myThread = nullptr;
	unblTime = false;
	
	lock;
	id = ++ID0;
	allPCBs.pushBack(this);
    unlock;
}

PCB::~PCB() {
	if (stack != nullptr) { // Kernel thread does not have allocated stack
		lock;
		delete[] stack;
		unlock;
		stack = nullptr;
	}
}

void PCB::start() {
	lock;
	state = READY;
	Scheduler::put(this);
	unlock;
}

void PCB::runner() {
	running->myThread->run();
	lock;
	running->releaseWaiting();
	running->state = FINISHED;
	dispatch();
	unlock;
}

void idleMethod() {
	cout << "Started idle!" << endl;
	while(true);
}

PCB* PCB::getIdlePCB() {
	static PCB idlePCB(64,1,nullptr,idleMethod,IDLE);
	return &idlePCB;
}

void PCB::waitToComplete() {
	lock;
	if (PCB::running != this && state != FINISHED && state != IDLE) {
		PCB::running->state = BLOCKED;
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