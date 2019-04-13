#include "Locker.h"

bool Locker::lockFlag = false;

void Locker::lock() {
	lockFlag = true;
}

void Locker::unlock() {
	lockFlag = false;
}
