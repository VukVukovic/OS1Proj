#include "utils.h"
#include <stdlib.h>

void* operator new(unsigned size) {
    return malloc(size);
}

void operator delete(void *p) {
    free(p);
}