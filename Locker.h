#ifndef LOCKER_H
#define LOCKER_H

#include "Util.h"

class Locker {
public:
	static bool lockFlag;
	static void lock();
	static void unlock();
private:
	Locker() {}
};
#endif
