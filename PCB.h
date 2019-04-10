#ifndef PCB_H
#define PCB_H
#include "Thread.h"

class PCB {
public:
	unsigned *stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned zavrsio;
	int kvant;

public:
	PCB();
	PCB(StackSize stackSize, Time timeSlice, void (*body)());
};



#endif
