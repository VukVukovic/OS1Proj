#include "utils.h"

volatile int lockCnt = 0;
volatile bool changeWaiting = false;