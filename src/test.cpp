#include "Thread.h"
#include <iostream.h>

#include "semaphor.h"
#include <stdlib.h>

int syncPrintf(const char *format, ...);

class Thread2 : public Thread {
public:
	Thread2() : Thread() {}
	~Thread2() { waitToComplete(); }
protected:
	void run();
};

void Thread2::run() {
	syncPrintf("Thread 2!\n");
	for (int i=0;i<100000;i++)
		for (int j=0;j<10000;j++);
	syncPrintf("Thread 2 finished!\n");
}

class Thread1 : public Thread {
public:
	Thread1() : Thread() {}
	~Thread1() { waitToComplete(); }
protected:
	void run();
};

void Thread1::run() {
	syncPrintf("Thread 1!\n");
	Thread2 thr;
	thr.start();
}

void finishHandler() {
	syncPrintf("Child thread finished!\n");
}

void tick() {}

int userMain(int argc, char* argv[]) {
	Thread2 t;
	t.start();
	Semaphore sleep(0);
	sleep.wait(20);
	syncPrintf("Taking too long, kill!\n");
	t.signal(0);
	return 0;
}
