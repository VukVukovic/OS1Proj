#include "Thread.h"
#include "utils.h"
#include <iostream.h>
#include "utils.h"

int syncPrintf(const char *format, ...);

/*
 	 Test: razlicita vremena izvrsavanja i velicine steka
*/

volatile Time ts;

void tick()
{
	syncPrintf("timeSlice=%d\n",ts);
}

class TestThread : public Thread
{
private:
	Time myTimeSlice;
public:

	TestThread(StackSize stackSize, Time timeSlice): Thread(stackSize,timeSlice), myTimeSlice(timeSlice) {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	for(unsigned i=0;i<3200;i++)
	{
		for(unsigned int j=0;j<3200;j++)
		{
			lock;
			ts = myTimeSlice;
			unlock;
		}
	}
}


int userMain(int argc, char** argv)
{
	syncPrintf("Test starts.\n");
	TestThread t1(64,1), t2(4096,32), t3(1024,16), t4(4096,0);
	t1.start();
	t2.start();
	t3.start();
	t4.start();
	t1.waitToComplete();
	t2.waitToComplete();
	t3.waitToComplete();
	t4.waitToComplete();
	syncPrintf("Test ends.\n");
	return 0;
}
