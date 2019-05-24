#include "Util.h"
#include <dos.h>

static unsigned oldTimerOFF=0, oldTimerSEG=0;

pInterrupt oldTimerRoutine;

void interrupt timer(...);

void inicTimerInterrupt() {
	oldTimerRoutine = getvect(8);
	setvect(0x08, timer);
	setvect(0x60, oldTimerRoutine);
}

void restoreTimerInterrupt() {
	setvect(0x08, oldTimerRoutine);
}
