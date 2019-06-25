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

extern volatile int lockCnt;
void dispatch ();

#define lock ++lockCnt
#define unlock if (--lockCnt == 0 && PCB::changeWaiting) { dispatch(); }
#define locked (lockCnt>0)

#endif