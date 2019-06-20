#include <iostream.h>
#include "utils.h"
#include "timer.h"

int main() {
    inicTimerInterrupt();

    restoreTimerInterrupt();
    return 0;
}