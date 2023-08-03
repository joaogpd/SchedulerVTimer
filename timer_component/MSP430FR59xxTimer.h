
#ifndef MSP430FR59XXTIMER
#define MSP430FR59XXTIMER
#include <msp430.h>
#include <stdint.h>

#ifndef MAX_VT_TASKS
#define MAX_VT_TASKS 5
#endif

#ifdef __cplusplus
extern "C" {
#endif

void startClockTimer();

#ifdef __cplusplus
}
#endif

#endif
