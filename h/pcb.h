#ifndef PCB_H
#define PCB_H
#include "utils.h"
#include "Thread.h"
#include "list.h"

enum State{READY, RUNNING, BLOCKED, FINISHED, IDLE};

class PCB {
public:
	volatile static PCB* running;
	volatile static List<PCB*> PCBlist;

	List<PCB*> waiting;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	int lockCount;
	State state;
	Time timeSlice;
	Thread *myThread;

	bool unblockedTime;
	
 	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)() = PCB::runner, State s = READY);
	
	static void runner();
	static PCB* getIdlePCB();

	void waitToComplete();
	void start();
	ID getId() { return id; }
	static ID getRunningId();
	static Thread* getThreadById(ID id);

	void unblock();
	void block();

	~PCB();
private:
	void releaseWaiting();
	volatile static ID ID0;
	ID id;
	
};
#endif