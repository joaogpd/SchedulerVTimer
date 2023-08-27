
#ifndef SCHEDULERVTIMER
#define SCHEDULERVTIMER
#include <Arduino.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

// Platforms
#define ATMEGA328P 0
#define MSP430FR59XX 1

#ifndef MAX_APP_TASKS
#define MAX_APP_TASKS 10
#endif

#ifndef MAX_VT_TASKS
#define MAX_VT_TASKS 5
#endif

#ifndef PLATFORM
#define PLATFORM ATMEGA328P
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {app=0,vtimer} taskId_t;
typedef void (*task_ptr_t)(uint8_t);

void setupTasks(taskId_t taskType,...);
int8_t getTaskIdx(taskId_t taskType, task_ptr_t fx);
void postVTTaskById(uint8_t taskId);
int8_t postTask(taskId_t taskType, task_ptr_t fx, uint8_t arg1);
void startVTimer(task_ptr_t fx, uint16_t tmilli, uint8_t arg1);
void stopVTimer(task_ptr_t fx);
void procTasks();
void initSchedulerVTTimer();

#ifdef __cplusplus
}
#endif

#endif
