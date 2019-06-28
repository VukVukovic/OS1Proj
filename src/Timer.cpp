#include "timer.h"
#include "pcb.h"
#include "utils.h"
#include "SCHEDULE.H"
#include <dos.h>
//#include <iostream.h>

pInterrupt oldTimerRoutine;

void inicTimerInterrupt() {
	oldTimerRoutine = getvect(8);
	setvect(8, timer);
}

void restoreTimerInterrupt() {
    disableInterrupts;
	setvect(8, oldTimerRoutine);
    enableInterrupts;
}

unsigned tsp;
unsigned tss;
unsigned tbp;
volatile int timeLeft = defaultTimeSlice;

void tick();

void interrupt timer(...){
    if (!explicitCall) {
        (*oldTimerRoutine)();
        tick();
    }

	if (!explicitCall && timeLeft>0)
		timeLeft--;

    if (explicitCall || timeLeft == 0) {
        explicitCall = false;
        if (!locked) {
            changeWaiting = false;
            asm {
                mov tsp, sp
                mov tss, ss
                mov tbp, bp
            }

            PCB::running->sp = tsp;
            PCB::running->ss = tss;
            PCB::running->bp = tbp;

            if (PCB::running->state == RUNNING) {
                PCB::running->state = READY;
                Scheduler::put((PCB*) PCB::running);
            }

            PCB::running = Scheduler::get();

            if (PCB::running == nullptr) 
                PCB::running = PCB::getIdlePCB();
            else
                PCB::running->state = RUNNING;
            
            timeLeft = PCB::running->timeSlice;

            tsp = PCB::running->sp;
            tss = PCB::running->ss;
            tbp = PCB::running->bp;

            asm {
                mov sp, tsp
                mov ss, tss
                mov bp, tbp
            }
        } else
            changeWaiting=true;
    }
	
}