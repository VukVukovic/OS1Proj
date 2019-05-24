#ifndef SLPLIST_H
#define SLPLIST_H

#include "Util.h"
#include "PCB.h"
#include "Thread.h"

class SleepList {
	struct Node {
		PCB* slpcb;
		Time relTime;
		Node *next;
		Node(PCB* slpcb, Time relTime, Node *next) : slpcb(slpcb), relTime(relTime), next(next) {}
	};
	Node *first;
public:
	SleepList() : first(nullptr) {}
	bool finishedSleep() { return (first && first->relTime==0); }
	PCB* get();
	void decFirst() { if (first) first->relTime--; }
	void add(PCB* slpcb, Time timeToSleep);
	~SleepList();
private:
	SleepList(const SleepList&);
	SleepList& operator=(const SleepList&);
};
#endif
