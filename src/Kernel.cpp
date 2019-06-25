#include <iostream.h>
#include "utils.h"
#include "timer.h"
#include "Thread.h"
#include "pcb.h"

int userMain (int argc, char* argv[]);

class UserThread : public Thread {
	int argc;
	char **argv;
	int retCode;
public:
	UserThread(int argc, char** argv) : Thread() {
		this->argc = argc;
		this->argv = argv;
	}
	int returnCode() { return retCode; }
protected:
	void run();
};

void UserThread::run() {
	retCode = userMain(argc,argv);
}

int main(int argc, char* argv[]) {
    inicTimerInterrupt();
    PCB *kernelPCB = new PCB();
	PCB::quantCounter = 1;
    PCB::running = kernelPCB;

	UserThread user(argc, argv);
	user.start();
	while (1);
	int ret = user.returnCode();

    restoreTimerInterrupt();
    delete kernelPCB;
    return ret;
}