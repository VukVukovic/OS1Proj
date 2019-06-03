#include <iostream.h>
#include "Util.h"
#include "PCB.h"
#include "Timer.h"

int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	inicTimerInterrupt();
	PCB* userMainPCB = new PCB(0,10,nullptr);
	PCB::running = userMainPCB;
	PCB::quantCounter = 3;

	int result = userMain(argc, argv);

	restoreTimerInterrupt();

	cout << "FINISHING KERNEL!" << endl;
	delete userMainPCB;
	return result;
}

