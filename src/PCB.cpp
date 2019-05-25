#include "PCB.h"
#include "SCHEDULE.h"
#include "Util.h"
#include "SlpList.h"
#include "Locker.h"
#include <dos.h>
#include <iostream.h>

volatile PCB* PCB::running = nullptr;
volatile Time PCB::quantCounter=2;
volatile bool PCB::explicitDispatch=false;

ID PCB::ID0 = 0;

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	this->myThread = myThread;

	if (stackSize>0) {
		stack = new unsigned[stackSize/sizeof(unsigned)];
		stack[stackSize-1] = 0x200; // PSWI=1
		stack[stackSize-2] = FP_SEG(PCB::runner);
		stack[stackSize-3] = FP_OFF(PCB::runner);

		sp = FP_OFF(stack+stackSize-STACK_REG_OFFSET);
		ss = FP_SEG(stack+stackSize-STACK_REG_OFFSET);
		bp = sp;
		state = READY;
	} else {
		stack = nullptr;
		sp = ss = bp = 0;
		state = RUNNING;
	}

	if (timeSlice == 0) timeSlice=-1; // Threads with unlimited timeSlice
	this->timeSlice = timeSlice;

	id = ++ID0;
}

void PCB::runner() {
	running->myThread->run();
	running->state = FINISHED;
	dispatch();
}