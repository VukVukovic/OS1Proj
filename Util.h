#ifndef UTIL_H_
#define UTIL_H_

#define FP_SEG( fp )( (unsigned )( void _seg * )( void far * )( fp ))
#define FP_OFF( fp )( (unsigned )( fp ))

#define NULL 0L
#define nullptr 0L

#define true 1
#define false 0

typedef int bool;

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))

void inicTimerInterrupt();
void restoreTimerInterrupt();
#endif
