#ifndef PCB_H
#define PCB_H
#include "utils.h"
#include "Thread.h"

enum State{READY, RUNNING, BLOCKED, FINISHED};

class PCB {
public:
	volatile static PCB* running;
	volatile static Time quantCounter;
	volatile static bool timerCall, changeWaiting;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	State state;

	Time timeSlice;
	Thread *myThread;
	
 	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread);
	static void runner();
private:
	static ID ID0;
	ID id;
};
#endif