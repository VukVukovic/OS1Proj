#ifndef UTILS_H
#define UTILS_H

typedef int bool;
#define nullptr 0
#define true 1
#define false 0

#define disableInterrupts asm { pushf; cli; }
#define enableInterrupts asm popf

extern volatile int lockCnt;
extern volatile bool changeWaiting;
void dispatch ();

#define lock ++lockCnt
#define unlock if (--lockCnt == 0 && changeWaiting) { dispatch(); }
#define locked (lockCnt>0)

#endif