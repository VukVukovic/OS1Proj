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
	//stek za main nit ne smem da brisem!

	int result = userMain(argc, argv);

	restoreTimerInterrupt();

	cout << "FINISHING KERNEL!" << endl;
	//delete userMainPCB;
	cout << PCB::deletedPCBs << endl;
	return result;
}

