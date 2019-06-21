#include "pcb.h"
#include "SCHEDULE.h"
#include "utils.h"
#include <dos.h>

volatile PCB* PCB::running = nullptr;
volatile Time PCB::quantCounter = 2;
volatile bool PCB::timerCall = false;
volatile bool PCB::changeWaiting = false;

ID PCB::ID0 = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	this->myThread = myThread;

	stack = new unsigned[stackSize/sizeof(unsigned)];
	stack[stackSize-1] = 0x200; // PSWI=1
	stack[stackSize-2] = FP_SEG(PCB::runner);
	stack[stackSize-3] = FP_OFF(PCB::runner);

	sp = FP_OFF(stack+stackSize-12);
	ss = FP_SEG(stack+stackSize-12);
	bp = sp;
	state = READY;

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

void PCB::runner() {
	running->myThread->run();
	running->state = FINISHED;
	dispatch();
}