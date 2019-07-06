#ifndef SIGSTAT_H
#define SIGSTAT_H
#include "utils.h"

typedef unsigned SignalId;

class SignalStatus {
    unsigned stat;
public:
    SignalStatus(unsigned s) : stat(s) {}
    void reset(SignalId sigId) { if (sigId<16) stat &= ~(1 << sigId); }
    void set(SignalId sigId) { if (sigId<16) stat |= (1 << sigId); }
    bool status(SignalId sigId) { 
        if (sigId>=16) return false;
        return (stat&(1<<sigId))!=0;
    }
    void setAll() { stat = ~0U; }
    void clearAll() { stat = 0; }
};
#endif