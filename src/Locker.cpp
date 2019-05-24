#include "Locker.h"
#include "PCB.h"

volatile int Locker::lockFlag = 0;

void Locker::lock() {
	lockFlag++;
}

void Locker::unlock() {
	lockFlag--;
	if (lockFlag==0 && PCB::explicitDispatch)
		dispatch();
}
