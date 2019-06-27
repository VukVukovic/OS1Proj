#include "pcb.h"
#include "SCHEDULE.h"
#include "utils.h"
#include <dos.h>

volatile PCB* PCB::running = nullptr;
volatile Time PCB::quantCounter = defaultTimeSlice;
volatile bool PCB::timerCall = false;
volatile List<PCB*> PCB::PCBlist;
ID PCB::ID0 = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)(), State s) {
	this->myThread = myThread;

	lock;
	stack = new unsigned[stackSize/sizeof(unsigned)];
	unlock;
	stack[stackSize-1] = 0x200; // PSWI=1
	stack[stackSize-2] = FP_SEG(fun);
	stack[stackSize-3] = FP_OFF(fun);

	sp = FP_OFF(stack+stackSize-12);
	ss = FP_SEG(stack+stackSize-12);
	bp = sp;
	state = s;

	if (timeSlice == 0) timeSlice=-1; // Threads with unlimited timeSlice
	this->timeSlice = timeSlice;

    lock;
	id = ++ID0;
	PCBlist.pushBack(this);
    unlock;
}

PCB::PCB() {
    stack = nullptr;
    myThread = nullptr;
    sp = ss = bp = 0;
	state = RUNNING;
	timeSlice=defaultTimeSlice;
	
	lock;
	id = ++ID0;
    unlock;
}

PCB::~PCB() {
	if (stack!=nullptr) {
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
	running->state = FINISHED;
	running->releaseWaiting();
	dispatch();
}

void idleMethod() {
	while(true);
}

PCB* PCB::getIdlePCB() {
	static PCB idlePCB(20,1,nullptr,idleMethod,IDLE);
	return &idlePCB;
}

void PCB::waitToComplete() {
	lock;
	if (state!=FINISHED) {
		PCB::running->state = BLOCKED;
		waiting.pushBack((PCB*)PCB::running);
		dispatch();
	}
	unlock;
}

void PCB::releaseWaiting() {
	lock;
	for (List<PCB*>::Iterator it = waiting.begin(); it.exists(); ++it) {
		PCB* releasePCB = *it;
		releasePCB->state = READY;
		Scheduler::put(releasePCB);
		it.remove();
	}
	unlock;
}


ID PCB::getRunningId() {
	return PCB::running->id;
}

Thread* PCB::getThreadById(ID id) {
	Thread *thr = nullptr;
	lock;
	for (List<PCB*>::Iterator it = PCBlist.begin(); it.exists(); ++it)
		if ((*it)->getId() == id){
			thr = (*it)->myThread;
			break;
		}
	unlock;
	return thr;
}