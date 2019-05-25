#ifndef TIMER_H
#define TIMER_H

typedef void interrupt (*pInterrupt) (...);
void inicTimerInterrupt();
void restoreTimerInterrupt();
void interrupt timer(...);
#endif