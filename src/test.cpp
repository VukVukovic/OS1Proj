#include "Thread.h"
#include <iostream.h>

#include "semaphor.h"
#include <stdlib.h>
#include "event.h"
#include <stdio.h>

int syncPrintf(const char *format, ...);

void tick(){}

class TestThread : public Thread
{
	int i;
public:

	TestThread(int i): i(i), Thread() {}
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Started thread! %d\n", i);

	Semaphore sleep(0);
	sleep.wait(5);
	if (i>0) {
		TestThread t(i-1);
		t.start();
	}

	syncPrintf("Finished thread! %d\n", i);
}

void finishedChildHandler() {
	syncPrintf("Child finished!\n");
}

void finishedThread() {
	syncPrintf("Thread finished!\n");
}

int userMain(int argc, char** argv)
{
	syncPrintf("User main started!\n");

	TestThread t(5);
	t.start();
	t.registerHandler(1, finishedChildHandler);
	t.registerHandler(2, finishedThread);
	//t.blockSignalGlobally(0);
	//t.signal(0);
	t.waitToComplete();

	syncPrintf("User main finished!\n");
	return 0;
}