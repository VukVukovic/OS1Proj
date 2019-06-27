#include <iostream.h>
#include "utils.h"
#include "timer.h"
#include "Thread.h"
#include "pcb.h"
#include "user.h"

int main(int argc, char* argv[]) {
	PCB *kernelPCB = new PCB();
    PCB::running = kernelPCB;
	inicTimerInterrupt();
	
	UserThread user(argc, argv);
	user.start();
	user.waitToComplete();
	int ret = user.returnCode();

	lock;
	cout << "User main finished with code: " << ret << endl;
	unlock;

    restoreTimerInterrupt();
    delete kernelPCB;
    return 0;
}