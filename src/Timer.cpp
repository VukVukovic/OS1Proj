#include "timer.h"
#include <dos.h>

pInterrupt oldTimerRoutine;

void inicTimerInterrupt() {
	oldTimerRoutine = getvect(8);
	setvect(8, timer);
}

void restoreTimerInterrupt() {
	setvect(8, oldTimerRoutine);
}

void interrupt timer(...){
    oldTimerRoutine();
}