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
	while (1) {
		syncPrintf("Print %d\n", k);
		for (int i=0;i<10000;i++)
			for (int j=0;j<10000;j++);
	}
}

void signalHandler1() {
	syncPrintf("Handled signal 1\n");
}

void signalHandler5() {
	syncPrintf("Handled signal 5\n");
}

void tick() {}

int userMain(int argc, char* argv[]) {
	Semaphore sleep(0);

	Thread1 t1(1), t2(2);
	t1.start();
	t2.start();

	t1.registerHandler(1, signalHandler1);
	t1.registerHandler(5, signalHandler5);
	t2.registerHandler(5, signalHandler5);
	t2.registerHandler(5, signalHandler5);

	int k = 10;
	Thread::blockSignalGlobally(5);
	while (k--) {
		sleep.wait(10);
		t1.signal(5);
		t2.signal(5);
	}
	
	
	return 0;
}
