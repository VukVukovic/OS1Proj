#ifndef IVTENTRY
#define IVTENTRY

#include "intrtype.h"
typedef unsigned char IVTNo;
class KernelEv;

#define ENTRY_NUM 256

#define PREPAREENTRY(ivtNum, callOld)\
extern IVTEntry ivtEntry##ivtNum;\
void interrupt interruptEvent##ivtNum(...) {\
    ivtEntry##ivtNum.signal();\
    if (callOld) ivtEntry##ivtNum.oldRoutine();\
    dispatch();\
}\
IVTEntry ivtEntry##ivtNum(ivtNum, interruptEvent##ivtNum);

class IVTEntry {
    IVTNo num;
    KernelEv *event;
    pInterrupt newRoutine;
public:
    pInterrupt oldRoutine;
    static IVTEntry *ivtable[ENTRY_NUM];
    
    IVTEntry(IVTNo num, pInterrupt newRoutine);
    ~IVTEntry();

    void signal();

    void setEvent(KernelEv *event);
    void clearEvent();
};
#endif