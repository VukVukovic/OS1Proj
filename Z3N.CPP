#include <iostream.h>
#include <stdio.h>
#include "PCB.h"
#include "Util.h"
#include "SCHEDULE.H"
#include "Thread.h"
#include "List.h"
#include "SlpList.h"

// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

class Nit1 : public Thread {
public:
	Nit1(StackSize stackSize, Time timeSlice):Thread(stackSize, timeSlice){}
protected:
	void run() {
		printf("A went to sleep!");
		sleep(100);
		printf("A woken up!");
		for (int i =0; i < 30; ++i) {
			// lock
				cout<<"u a() i = "<<i<<endl;
			//	unlock
				for (int k = 0; k<10000; ++k)
					for (int j = 0; j <30000; ++j);
			}
	}
};

class Nit2 : public Thread {
public:
	Nit2(StackSize stackSize, Time timeSlice):Thread(stackSize, timeSlice){}
protected:
	void run() {
		for (int i =0; i < 30; ++i) {
				//lock
				cout<<"u b() i = "<<i<<endl;
				//unlock
				for (int k = 0; k<10000; ++k)
					for (int j = 0; j <30000; ++j);
			}
	}
};

unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 1;
volatile bool zahtevana_promena_konteksta = 0;

void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta) {
		brojac--;
		PCB::decSleepingWake();
	}
	if (brojac == 0 || zahtevana_promena_konteksta) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if (!PCB::running->finished && !PCB::running->sleeping)
			Scheduler::put((PCB*) PCB::running);

		PCB::running = Scheduler::get();

		//printf("Prelazim na nit ID=%d\n", PCB::running->id);
		//asm cli;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		brojac = PCB::running->kvant;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}     
	} 
    
	// poziv stare prekidne rutine koja se 
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida	
if(!zahtevana_promena_konteksta) asm int 60h;
		                                              
	zahtevana_promena_konteksta = 0;

}

void dispatch(){ // sinhrona promena konteksta 
	asm cli;
	zahtevana_promena_konteksta = 1;
	timer();
	asm sti;
}

void doSomething(){
	lock
	PCB::running = new PCB();
	Thread *t1 = new Nit1(1024, 1);
	Thread *t2 = new Nit2(1024, 1);
	t1->start();
	t2->start();
	unlock

	lock
	printf("Put to sleep!");
	unlock
	Thread::sleep(50);
	lock
	printf("Woken up!");
	unlock

	List<int> lst;
	lst.append(5);
	lst.append(6);
	lst.add(7);
	lst.add(8);
	lst.append(9);

	for (lst.begin();lst.exists();lst.next()) {
		printf("%d ", lst.get());
	}

	for (int i = 0; i < 30; ++i) {
  	lock
  	cout<<"main "<<i<<endl;
  	unlock

	for (int j = 0; j< 30000; ++j)
		for (int k = 0; k < 30000; ++k);
	}
	cout<<"Happy End"<<endl;
}

int main(){
	inicTimerInterrupt();
	
	doSomething();

	restoreTimerInterrupt();
	return 0;
}

