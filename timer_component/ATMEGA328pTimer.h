
#ifndef ATMEGA328PTIMER
#define ATMEGA328PTIMER
#include <Arduino.h>
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
