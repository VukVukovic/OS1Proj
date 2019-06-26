#include "utils.h"
#include <stdlib.h>

volatile int lockCnt = 0;
volatile bool changeWaiting = false;

void* operator new(unsigned size) {
    lock;
    void* ret = malloc(size);
    unlock;
    return ret;
}

void operator delete(void *p) {
    lock;
    free(p);
    unlock;
}