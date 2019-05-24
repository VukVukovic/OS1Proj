#include "PCB.h"
#include "SCHEDULE.h"
#include "Util.h"
#include "SlpList.h"
#include "Locker.h"
#include <iostream.h>

volatile PCB* PCB::running = nullptr;
volatile int PCB::quantCounter=8;
volatile bool PCB::explicitDispatch=false;
ID PCB::ID0 = 0;
//SleepList PCB::sleepList;

PCB::PCB() {
	sp = ss = bp = 0;
	finished = false;
	timeSlice=2;
	stack = nullptr;
	id = ++ID0;
}

PCB::PCB(StackSize stackSize, Time timeSlice, Thread *myThread) {
	this->myThread = myThread;

	stack = new unsigned[stackSize];
	stack[stackSize-1] = 0x200;
	stack[stackSize-2] = FP_SEG(PCB::runner);
	stack[stackSize-3] = FP_OFF(PCB::runner);

	sp = FP_OFF(stack+stackSize-STACK_REG_OFFSET);
	ss = FP_SEG(stack+stackSize-STACK_REG_OFFSET);
	bp = sp;
	finished = false;
	//sleeping = false;
	this->timeSlice = timeSlice;
	id = ++ID0;
}

void PCB::runner() {
	PCB::running->myThread->run();
	running->finished = true;
	dispatch();
}

/*
void PCB::sleep(Time timeToSleep) {
	PCB::running->sleeping=true;
	sleepList.add((PCB*)PCB::running, timeToSleep);
	dispatch();
}

void PCB::decSleepingWake() {
	sleepList.decFirst();
	while (sleepList.finishedSleep()) {
		PCB *wokenup = PCB::sleepList.get();
		wokenup->sleeping = false;
		Scheduler::put(wokenup);
	}
} */

unsigned tsp;
unsigned tss;
unsigned tbp;

void interrupt timer(...){
	if(!PCB::explicitDispatch) asm int 60h;

	if (!PCB::explicitDispatch && PCB::quantCounter>0)
		PCB::quantCounter--;

	if ((PCB::quantCounter == 0 && PCB::running->timeSlice>0) || PCB::explicitDispatch) {
		if (!Locker::locked()) {
			PCB::explicitDispatch = false;
			asm {
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if (!PCB::running->finished)
				Scheduler::put((PCB*) PCB::running);

			PCB::running = Scheduler::get();

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			PCB::quantCounter = PCB::running->timeSlice;

			asm {
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}
		}
		else
			PCB::explicitDispatch = true;
	}
}

