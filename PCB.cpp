#include "PCB.h"

#include "Util.h"

PCB::PCB() {
	sp = ss = bp = zavrsio =  0;
	kvant=10;
	stack = nullptr;
}

PCB::PCB(StackSize stackSize, Time timeSlice, void (*body)()) {
	unsigned* st1 = new unsigned[1024];

	st1[1023] =0x200;//setovan I fleg u
	                      // pocetnom PSW-u za nit
	st1[1022] = FP_SEG(body);
	st1[1021] = FP_OFF(body);

	sp = FP_OFF(st1+1012);
	ss = FP_SEG(st1+1012);
	zavrsio = 0;
	kvant = timeSlice;
}
