#include "Thread.h"
#include "utils.h"
#include <iostream.h>

class Nit1 : public Thread {
public:
	Nit1():Thread(1024,1){}
protected:
	void run();
};

void Nit1::run() {
		for (int i = 0; i < 30; ++i) {
			lock;
			cout<<"u a() i = "<<i<<endl;
			unlock;
			//for (int k = 0; k<10000; ++k)
			//	for (int j = 0; j <30000; ++j);
		}
}

class Nit2 : public Thread {
public:
	Nit2():Thread(1024,5){}
protected:
	void run();
};

void Nit2::run() {
	for (int i =0; i < 30; ++i) {
		lock;
		cout<<"u b() i = "<<i<<endl;
		unlock;
		//for (int k = 0; k<10000; ++k)
		//	for (int j = 0; j <30000; ++j);
	}
}

int userMain (int argc, char* argv[]) {
    Nit1 n1; n1.start();
    Nit2 n2; n2.start();
	n1.waitToComplete();
	n2.waitToComplete();
    return 0;
}