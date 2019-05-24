#ifndef PCB_H
#define PCB_H
#include "Util.h"
#include "Thread.h"

class SleepList;

class PCB {
public:
	volatile static PCB* running;
	volatile static int quantCounter;
	volatile static bool explicitDispatch;
	//static SleepList sleepList;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	bool finished;
	//bool sleeping;
	int timeSlice;
	Thread *myThread;
	ID id;

	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread);
	static void runner();
	//static void sleep(Time timeToSleep);
	//static void decSleepingWake();
private:
	static ID ID0;
};

void interrupt timer();
#endif
