#include <iostream.h>
#include "utils.h"
#include "timer.h"
#include "Thread.h"
#include "pcb.h"

class Nit1 : public Thread {
public:
	Nit1():Thread(){}
protected:
	void run();
};

void Nit1::run() {
		for (int i =0; i < 30; ++i) {
			lock;
			cout<<"u a() i = "<<i<<endl;
			unlock;
			for (int k = 0; k<10000; ++k)
				for (int j = 0; j <30000; ++j);
		}
}

class Nit2 : public Thread {
public:
	Nit2():Thread(){}
protected:
	void run();
};

void Nit2::run() {
	for (int i =0; i < 30; ++i) {
		lock;
		cout<<"u b() i = "<<i<<endl;
		unlock;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

int main() {
    inicTimerInterrupt();
    PCB *kernelPCB = new PCB();
    PCB::running = kernelPCB;

    Nit1 n1; n1.start();
    Nit2 n2; n2.start();

    for (int i = 0; i < 30; ++i) {
		lock;
		cout<<"main "<<i<<endl;
		unlock;

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}

    restoreTimerInterrupt();
    delete kernelPCB;
    return 0;
}