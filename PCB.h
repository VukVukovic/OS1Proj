#ifndef PCB_H
#define PCB_H
#include "Util.h"
#include "Thread.h"

class PCB {
public:
	volatile static PCB* running;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	bool finished;
	int kvant;
	Thread *myThread;
	ID id;

	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread);
	static void runner();

private:
	static ID ID0;
};



#endif
