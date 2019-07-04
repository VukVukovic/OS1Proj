#include "Thread.h"
#include <iostream.h>

#include "semaphor.h"
#include <stdlib.h>
#include <assert.h>

int syncPrintf(const char *format, ...);

/*
 	 Test: Semafori sa spavanjem
*/

int t=-1;

Semaphore s(0);

class TestThread : public Thread
{
public:

	TestThread(): Thread(){}
	~TestThread()
	{
		waitToComplete();
	}
protected:
	void run();
};

void TestThread::run()
{
	syncPrintf("Thread waits for 10 units of time... %d\n", getId());
	if (t<0) t=0;
	s.wait(10);
	syncPrintf("Thread finished.\n");
	s.signal();
}

void tick()
{
	t++;
	if(t)
		syncPrintf("%d\n",t);
}

int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t1,t2,t3;
	t1.start();
	t2.start();
	t3.start();
	t1.waitToComplete();
	t2.waitToComplete();
	t3.waitToComplete();
	syncPrintf("Test ends.\n");
	return 0;
}