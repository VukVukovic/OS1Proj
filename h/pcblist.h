#ifndef PCBLIST_H
#define PCBLIST_H
#include "utils.h"

class PCB;

class PCBList {
    struct Elem {
        PCB *pcb;
        Elem *next;
        Elem(PCB *pcb, Elem *next=nullptr) : pcb(pcb), next(next) {}
    };

    Elem *head, *tail;
    friend class PCBIterator;
public:
    PCBList() : head(nullptr), tail(nullptr) {}
    void pushFront(PCB *pcb);
    void pushBack(PCB *pcb);
    PCB* popFront();
    bool empty() { return head == nullptr; }
    ~PCBList();

    class PCBIterator {
        PCBList *list;
        Elem *current;

    public:
    PCBIterator(PCBList *list, Elem *current) : list(list), current(current) {}
    PCB* operator*() { return current->pcb; }
    void operator++() { if (current!=nullptr) current = current->next; }
    void operator++(int k) { ++(*this); }
    bool exists() { return current!=nullptr;}
    };

    PCBIterator begin() { return PCBIterator(this, head); }
    
};
#endif