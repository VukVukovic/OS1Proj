//pretpostavljeni memorijski model: huge

#include <iostream.h>
#include <stdio.h>
#include "PCB.h"
#include "Util.h"
#include "SCHEDULE.H"

// Zabranjuje prekide
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti

PCB *p[3];
volatile PCB* running; 

unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 20;
volatile bool zahtevana_promena_konteksta = 0;

void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta) brojac--; 
	if (brojac == 0 || zahtevana_promena_konteksta) {
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		running->sp = tsp;
		running->ss = tss;
		running->bp = tbp;

		if (!running->zavrsio)
			Scheduler::put((PCB*) running);
		running = Scheduler::get();
  
		tsp = running->sp;
		tss = running->ss; 
		tbp = running->bp;

		brojac = running->kvant;

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

void exitThread(){
	running->zavrsio = 1;
	dispatch();
}  

void a(){
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"u a() i = "<<i<<endl;
		unlock
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void b(){
	for (int i =0; i < 30; ++i) {
		lock
		cout<<"u b() i = "<<i<<endl;
		unlock
		for (int k = 0; k<10000; ++k)
			for (int j = 0; j <30000; ++j);
	}
	exitThread();
}

void doSomething(){
	lock

	p[1] = new PCB(1024,40,a);
	cout<<"napravio a"<<endl;
	p[2] = new PCB(1024,20,b);
	cout<<"napravio b"<<endl;
	p[0] = new PCB();

	Scheduler::put(p[1]);
	Scheduler::put(p[2]);

	running = p[0];
	unlock

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

