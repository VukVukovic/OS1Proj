#ifndef PCB_H
#define PCB_H
#include "Util.h"
#include "Thread.h"
#include "List.h"

enum State{READY, RUNNING, BLOCKED, FINISHED};

class PCB {
public:
	volatile static PCB* running;
	volatile static Time quantCounter;
	volatile static bool explicitDispatch;
	volatile static List<PCB*> PCBList;
	static int deletedPCBs;

	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	State state;

	Time timeSlice;
	Thread *myThread;

	List<PCB*> waiting;
	
	void start();
	ID getID() { return id; }
	static Thread* getThreadById(ID id);
	static ID getRunningId();

	void waitToComplete();
	void wakeUpWaiting();

 	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread);
	static void runner();

	~PCB();
private:
	static ID ID0;
	ID id;
};
#endif
