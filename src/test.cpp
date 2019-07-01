#include "Thread.h"
#include <iostream.h>

int syncPrintf(const char *format, ...);
void tick() {}

class TestThread : public Thread {
	int i;
public:
	TestThread(int i) : Thread(64, 0), i(i) {}
protected:
	void run();
};

void TestThread::run() {
	syncPrintf("%d start\n", i);
	dispatch();
	for (int j=0;j<9000;j++)
		for (int k=0;k<9000;k++);
	dispatch();
	syncPrintf("%d end\n", i);
}

int userMain(int argc, char* argv[]) {
	const int n = 36;
	Thread *thr[n];

	syncPrintf("Started\n");

	for (int i=0;i<n;i++) {
		thr[i] = new TestThread(i);
		thr[i]->start();
	}

	for (i=0;i<n;i++)
		thr[i]->waitToComplete();

	syncPrintf("Finished\n");

	return 0;
}