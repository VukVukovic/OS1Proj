#ifndef UTILS_H
#define UTILS_H

typedef int bool;
#define nullptr 0
#define true 1
#define false 0

void* operator new(unsigned size);
void operator delete(void *p);

#define disableInterrupts asm { pushf; cli; }
#define enableInterrupts asm popf

#endif