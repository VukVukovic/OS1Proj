#include "pcb.h"
#include "SCHEDULE.h"
#include "utils.h"
#include <dos.h>

volatile PCB* PCB::running = nullptr;
volatile Time PCB::quantCounter = 0;
volatile bool PCB::timerCall = false;
PCB* PCB::idlePCB = nullptr;

ID PCB::ID0 = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)(), State s) {
	this->myThread = myThread;

	stack = new unsigned[stackSize/sizeof(unsigned)];
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
    unlock;
}

PCB::PCB() {
    stack = nullptr;
    myThread = nullptr;
    sp = ss = bp = 0;
	state = RUNNING;
	timeSlice=2;
}

PCB::~PCB() {
	if (stack!=nullptr) {
		delete[] stack;
		stack = nullptr;
	}
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
	}
	unlock;
}