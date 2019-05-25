#ifndef PCB_H
#define PCB_H
#include "Util.h"
#include "Thread.h"

enum State{READY, RUNNING, BLOCKED, FINISHED};

class PCB {
public:
	volatile static PCB* running;
	volatile static Time quantCounter;
	volatile static bool explicitDispatch;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	State state;

	Time timeSlice;
	Thread *myThread;
	
	ID getID() { return id; }
 	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread);
	static void runner();
private:
	static ID ID0;
	ID id;
};
#endif
