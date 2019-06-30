#include "Thread.h"
#include <iostream.h>

int syncPrintf(const char *format, ...);
void tick() {}

class TestThread : public Thread{
public:
	int i;
	TestThread(int i, StackSize sz, Time slice) : Thread(sz, slice), i(i) {}
protected:
	void run();
};

void TestThread::run() {
	syncPrintf("Finished %d\n", getId());
}

int userMain(int argc, char* argv[]) {
	TestThread *threads[16];
	for (int i=0;i<16;i++) {
		threads[i] = new TestThread(i, 64*(i+1), i);
		threads[i]->start();
	}

	for (i=0;i<16;i++)
		threads[i]->waitToComplete();

	for (i=0;i<16;i++)
		delete threads[i];

	return 0;
}