#include "Util.h"
#include "Locker.h"
#include <stdlib.h>
#include <iostream.h>

/*
void* operator new(unsigned size) {
    lock;
    void *p = malloc(size);
    unlock;
    return p;
}


void* operator new[](unsigned size) {
    lock;
    void *p = malloc(size);
    unlock;
    return p;
}

void operator delete(void *p) {
    lock;
    cout << "Not Deleting" << endl;
    //free(p);
    unlock;
}

void operator delete[](void *p) {
    lock;
    free(p);
    unlock;
}
*/