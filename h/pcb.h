#ifndef PCB_H
#define PCB_H
#include "utils.h"
#include "Thread.h"
#include "list.h"
#include "sigstat.h"

const StackSize minStackSize = 256;
const StackSize maxStackSize = 65536;

enum State{CREATED, READY, RUNNING, BLOCKED, FINISHED, IDLE};

class PCB {
public:
	volatile static PCB* running;
	volatile static List<PCB*> allPCBs;

	List<PCB*> waiting;
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	int lockCount;
	State state;
	int timeSlice;
	Thread *myThread;

	bool unblTime;

	SignalStatus localPermission;
	volatile static SignalStatus globalPermission;
	List<SignalHandler> handlers[16];
	List<SignalId> activeSignals;
	ID parent;
	
 	PCB();
	PCB(StackSize stackSize, Time timeSlice, Thread *myThread, void (*fun)() = PCB::runner, State s = CREATED);
	
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

	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);

	static bool handleSignals();
	static void killThread(PCB *pcb);

private:
	void releaseWaiting();
	volatile static ID ID0;
	ID id;
};
#endif