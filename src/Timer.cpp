#include "timer.h"
#include "pcb.h"
#include "utils.h"
#include "SCHEDULE.H"
#include <dos.h>
#include <iostream.h>

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

void interrupt timer(...){
    if (!PCB::timerCall)
        (*oldTimerRoutine)();

	if (!PCB::timerCall && PCB::quantCounter>0)
		PCB::quantCounter--;

    if (PCB::timerCall || PCB::quantCounter == 0) {
        PCB::timerCall = false;
        if (!locked) {
            PCB::changeWaiting=false;
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
            PCB::running->state = RUNNING;
            
            PCB::quantCounter = PCB::running->timeSlice;

            tsp = PCB::running->sp;
            tss = PCB::running->ss;
            tbp = PCB::running->bp;

            asm {
                mov sp, tsp
                mov ss, tss
                mov bp, tbp
            }
        } else
            PCB::changeWaiting=true;
    }
	
}