#ifndef PCB_H
#define PCB_H
#include "utils.h"
#include "Thread.h"
#include "list.h"
#include "pcblist.h"

const StackSize minStackSize = 64;
const StackSize maxStackSize = 65536;

enum State{READY, RUNNING, BLOCKED, FINISHED, IDLE};

class PCB {
public:
	volatile static PCB* running;
	volatile static PCBList allPCBs;

	PCBList waiting;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	int lockCount;
	State state;
	int timeSlice;
	Thread *myThread;

	bool unblTime;
	
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
	
	void unblockedTime(bool flag) { unblTime = flag; }
	bool unblockedTime() { return unblTime; }

	~PCB();
private:
	void releaseWaiting();
	volatile static ID ID0;
	ID id;
	
};
#endif