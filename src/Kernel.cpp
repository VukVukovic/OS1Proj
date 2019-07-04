#include <iostream.h>
#include "utils.h"
#include "timer.h"
#include "Thread.h"
#include "pcb.h"
#include "userthr.h"

#include "list.h"

int main(int argc, char* argv[]) {
	PCB kernelPCB;
    PCB::running = &kernelPCB;
	inicTimerInterrupt();
	
	UserThread user(argc, argv);
	user.start();
	user.waitToComplete();
	int ret = user.returnCode();

	lock;
	cout << "User main finished with code: " << ret << endl;
	unlock;

    restoreTimerInterrupt();

	cout << endl; 
    return 0;
}