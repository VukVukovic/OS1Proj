#include "timer.h"
#include "pcb.h"
#include "utils.h"
#include "SCHEDULE.H"
#include <dos.h>
#include "kersem.h"
#include "intrtype.h"

pInterrupt oldTimerRoutine;

void inicTimerInterrupt() {
    disableInterrupts;
	oldTimerRoutine = getvect(8);
	setvect(8, timer);
    enableInterrupts;
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
PCB* toDelete;

void tick();

void interrupt timer(...){    
    if (!explicitCall) {
        (*oldTimerRoutine)();
        KernelSem::tick();
        tick();
    }

	if (!explicitCall && timeLeft>0)
		timeLeft--;

    if (explicitCall || (timeLeft == 0 && !locked)) {
        explicitCall = false;
        changeWaiting = false;
        asm {
            mov tsp, sp
            mov tss, ss
            mov tbp, bp
        }

        PCB::running->sp = tsp;
        PCB::running->ss = tss;
        PCB::running->bp = tbp;
        PCB::running->lockCount = lockCnt;

        if (PCB::running->state == RUNNING) {
            PCB::running->state = READY;
            Scheduler::put((PCB*) PCB::running);
        }

        toDelete = nullptr;
        do {
            PCB::running = Scheduler::get();

            if (PCB::running == nullptr)
                PCB::running = PCB::getIdlePCB();
            else
                PCB::running->state = RUNNING;
                
            timeLeft = PCB::running->timeSlice;

            tsp = PCB::running->sp;
            tss = PCB::running->ss;
            tbp = PCB::running->bp;
            lockCnt = PCB::running->lockCount;

            asm {
                mov sp, tsp
                mov ss, tss
                mov bp, tbp
            }

            if (toDelete != nullptr) {
                PCB::killThread(toDelete);
                toDelete = nullptr;
            }

            if (PCB::running->state==IDLE) break;
            if (PCB::handleSignals())
                toDelete = (PCB*)PCB::running;
            // Delete thread's stack from other's contex
        } while (toDelete!=nullptr);
    } else if (timeLeft == 0 && locked)
        changeWaiting = true;
}