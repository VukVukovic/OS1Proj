#include "SlpList.h"

PCB* SleepList::get() {
	PCB *ret = first->slpcb;
	Node *del = first;
	first = first->next;
	delete del;
	return ret;
}
void SleepList::add(PCB* slpcb, Time timeToSleep) {
	Node *newnode = new Node(slpcb, timeToSleep, nullptr);
	if (!first)
		first = newnode;
	else if (timeToSleep < first->relTime) {
		first->relTime -= timeToSleep;
		newnode->next = first;
		first = newnode;
	}
	else {
		Node *prev = nullptr, *p = first;
		while (p && p->relTime<newnode->relTime) {
			newnode->relTime -= p->relTime;
			prev = p;
			p = p->next;
		}
		if (p) p->relTime -= newnode->relTime;
		prev->next = newnode;
		newnode->next = p;
	}
}
SleepList::~SleepList() {
	if (first) {
		Node *del = first;
		first = first->next;
		delete del;
	}
}
