#include "Timer.h"
#include "PCB.h"
#include "Locker.h"
#include "SCHEDULE.H"
#include <dos.h>

pInterrupt oldTimerRoutine;

void inicTimerInterrupt() {
	oldTimerRoutine = getvect(8);
	setvect(8, timer);
}

void restoreTimerInterrupt() {
	setvect(8, oldTimerRoutine);
}

unsigned tsp;
unsigned tss;
unsigned tbp;

void interrupt timer(...){
    bool ticked = !PCB::explicitDispatch;

    if (ticked)  (*oldTimerRoutine)();
	if (ticked && PCB::quantCounter>0)
		PCB::quantCounter--;

	if (PCB::explicitDispatch || PCB::quantCounter == 0) {
        if (!locked) {
            PCB::explicitDispatch = false;
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
            // ADD SUPPORT FOR DUMMY PCB (NO READY PCBs)

            tsp = PCB::running->sp;
            tss = PCB::running->ss;
            tbp = PCB::running->bp;

            PCB::quantCounter = PCB::running->timeSlice;

            asm {
                mov sp, tsp
                mov ss, tss
                mov bp, tbp
            }
        } else 
            PCB::explicitDispatch = true; 
	}
}
