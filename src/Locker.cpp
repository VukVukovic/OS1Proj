#include "Locker.h"
#include "PCB.h"

volatile int Locker::lockCnt = 0;
//volatile int Locker::flags = 0;
//volatile int Locker::disabled = false;


void Locker::_lock() {
	lockCnt++;
}

void Locker::_unlock() {
	lockCnt--;
	if (lockCnt==0 && PCB::explicitDispatch)
		dispatch();
}

/*
void Locker::disableInterrupts() {
	if (disabled) return;
	disabled = true;

	int _flags;
	asm {
		pushf
		pop _flags
		cli
	}
	flags = _flags;
}

void Locker::enableInterrupts() {
	if (!disabled) return;
	int _flags = flags;
	asm {
		push _flags
		popf
	}
	disabled = false;
}
*/