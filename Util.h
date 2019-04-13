#ifndef UTIL_H_
#define UTIL_H_

#define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
#define FP_OFF( fp )( (unsigned )( fp ))

#define NULL 0L
#define nullptr 0L

typedef int bool;
#define true 1
#define false 0

#define STACK_REG_OFFSET 12

void inicTimerInterrupt();
void restoreTimerInterrupt();
#endif
