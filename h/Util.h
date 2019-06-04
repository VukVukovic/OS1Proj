#ifndef UTIL_H
#define UTIL_H

#define NULL 0L
#define nullptr 0L

typedef int bool;
#define true 1
#define false 0

#define STACK_REG_OFFSET 12

void* operator new(unsigned size);
//void* operator new[](unsigned size);
void operator delete(void *p);
//void operator delete[](void *p);
#endif