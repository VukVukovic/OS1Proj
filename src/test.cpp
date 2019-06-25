#include "Thread.h"
#include "utils.h"
#include "pcb.h"
#include <iostream.h>

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

int userMain (int argc, char* argv[]) {
    Nit1 n1; n1.start();
    Nit2 n2; n2.start();
    
    for (int i = 0; i < 30; ++i) {
		lock;
		cout<<"user main "<<i<<endl;
		unlock;

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}

    return 0;
}