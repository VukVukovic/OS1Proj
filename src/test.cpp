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
	syncPrintf("Thread %d!\n", getId());
	for (int i=0;i<10000;i++)
		for (int j=0;j<10000;j++);
	syncPrintf("Thread %d finished!\n", getId());
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
	Thread2 thr[10];
	for (int i=0;i<10;i++)
		thr[i].start();

	Semaphore sleep(0);
	sleep.wait(5);
	syncPrintf("Taking too long, kill!\n");
	
	thr[0].signal(0);
	thr[1].signal(0);
	thr[2].signal(0);
	thr[3].signal(0);

	return 0;
}
