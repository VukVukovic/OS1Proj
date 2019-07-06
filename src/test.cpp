#include "Thread.h"
#include <iostream.h>

#include "semaphor.h"
#include <stdlib.h>

int syncPrintf(const char *format, ...);

class Thread1 : public Thread {
	int k;
public:
	Thread1(int k) : Thread(), k(k) {}
	~Thread1() { waitToComplete(); }
protected:
	void run();
};

void Thread1::run() {
	syncPrintf("Child thread executed!\n");
}

void finishHandler() {
	syncPrintf("Child thread finished!\n");
}

void tick() {}

int userMain(int argc, char* argv[]) {
	Thread1 t1(1);
	Thread::getThreadById(Thread::getRunningId())->registerHandler(2, finishHandler);
	t1.start();
	return 0;
}
