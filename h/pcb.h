#ifndef PCB_H
#define PCB_H
#include "utils.h"
#include "Thread.h"
#include "list.h"

enum State{READY, RUNNING, BLOCKED, FINISHED, IDLE};

class PCB {
public:
	volatile static PCB* running;
	volatile static Time quantCounter;
	volatile static bool timerCall;

	static PCB* idlePCB;

	List<PCB*> waiting;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	State state;

	Time timeSlice;
	Thread *myThread;
	
 	PCB();
	PCB(bool idle);
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)() = PCB::runner, State s = READY);
	static void runner();
	static PCB* getIdlePCB();

	void waitToComplete();

	~PCB();
private:
	void releaseWaiting();
	static ID ID0;
	ID id;
};
#endif