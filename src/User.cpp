#include <iostream.h>
#include "Locker.h"
#include "Thread.h"

class Nit1 : public Thread {
public:
	Nit1(StackSize stackSize, Time timeSlice):Thread(stackSize, timeSlice){}
	~Nit1() {}//waitToComplete();}
protected:
	void run();
};

class Nit2 : public Thread {
public:
	Nit2(StackSize stackSize, Time timeSlice):Thread(stackSize, timeSlice){}
	~Nit2() {}//waitToComplete();}
protected:
	void run();
};

void Nit2::run() {
	for (int i = 0; i < 10; ++i) {
		lock;
		cout<<"u b() i = "<<i<<endl;
		unlock;
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
}

void Nit1::run() {
		for (int i = 0; i < 10; ++i) {
			lock;
			cout<<"u a() i = "<<i<<endl;
			unlock;
			for (int k = 0; k<10000; ++k)
				for (int j = 0; j <30000; ++j);
		}
}

int userMain(int argc, char* argv[])
{
    Nit1 t1 = Nit1(1024, 1);
	Nit2 t2 = Nit2(1024, 1);
	t1.start();
	t2.start();

	for (int i = 0; i < 10; ++i) {
		int k = lockCnt;
		lock;
		cout<<"main "<<i<<" "<<k<<endl;
		unlock;

		for (int j = 0; j< 30000; ++j)
			for (int k = 0; k < 30000; ++k);
	}

   //t1.waitToComplete();
   lock;
   cout << "HEJ!"<<endl;
   unlock;
	//t2.waitToComplete();

	lock;
	cout<<"Happy End"<<endl;
	unlock;
	return 0;
}