#ifndef LOCKER_H
#define LOCKER_H

#include "Util.h"

class Locker {
public:
	volatile static int lockFlag;
	static void lock();
	static void unlock();
	static bool locked() { return lockFlag>0; }
private:
	Locker() {}
};
#endif
