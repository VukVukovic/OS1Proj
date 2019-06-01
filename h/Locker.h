#ifndef LOCKER_H
#define LOCKER_H

#include "Util.h"

#define disableInterrupts asm cli
#define enableInterrupts asm sti
#define lock Locker::_lock()
#define unlock Locker::_unlock()
#define locked Locker::_locked()

class Locker {
public:
	volatile static int lockCnt;
	static void _lock();
	static void _unlock();
	static bool _locked() { return lockCnt>0; }

	//volatile static int flags;
	//volatile static bool disabled;
	//static void disableInterrupts();
	//static void enableInterrupts();
private:
	Locker() {}
};
#endif
