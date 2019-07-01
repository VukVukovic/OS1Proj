#include "pcblist.h"

void PCBList::pushFront(PCB *pcb) {
    lock;
    Elem *newelem = new Elem(pcb, head);
    unlock;
    if (empty()) tail = newelem;
    head = newelem;
}

void PCBList::pushBack(PCB *pcb) {
    lock;
    Elem *newelem = new Elem(pcb);
    if (empty()) head = newelem;
    else tail->next = newelem;
    tail = newelem;
    unlock;
}

PCB* PCBList::popFront() {
    if (empty()) return nullptr;
    PCB *ret = head->pcb;
    Elem *toDelete = head;
    head = head->next;
    lock;
    delete toDelete;
    unlock;
    if (head == nullptr) tail = nullptr;
    return ret;
}

PCBList::~PCBList() {
    while (head != nullptr) {
        Elem *toDelete = head;
        head = head->next;
        lock;
        delete toDelete;
        unlock;
    }
    head = tail = nullptr;
}