#include <iostream.h>
#include <stdlib.h>
#include "Util.h"
#include "Thread.h"
#include "Locker.h"
#include "PCB.h"
#include "Timer.h"
#include "List.h"

class Nit1 : public Thread {
public:
	Nit1(StackSize stackSize, Time timeSlice):Thread(stackSize, timeSlice){}
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
	Nit2(StackSize stackSize, Time timeSlice):Thread(stackSize, timeSlice){}
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
void doSomething(){
	Thread *t1 = new Nit1(1024, 1);
	Thread *t2 = new Nit2(1024, 1);
	t1->start();
	t2->start();

	for (int i = 0; i < 30; ++i) {
		int k = lockCnt;
		lock;
		cout<<"main "<<i<<" "<<k<<endl;
		unlock;

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}

	lock;
	cout<<"Happy End"<<endl;
	unlock;
}

int main(){
	inicTimerInterrupt();
	PCB::running = new PCB(0,10,nullptr);
	PCB::quantCounter = 3;
	doSomething();

	restoreTimerInterrupt();
	return 0;
}

