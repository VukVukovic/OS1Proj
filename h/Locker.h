#ifndef LOCKER_H
#define LOCKER_H

#include "Util.h"
#include "PCB.h"

#define disableInterrupts asm { pushf; cli; }
#define enableInterrupts asm popf

extern volatile int lockCnt;
#define lock ++lockCnt
#define unlock if (--lockCnt == 0 && PCB::explicitDispatch) { dispatch(); }
#define locked (lockCnt>0)
#endif