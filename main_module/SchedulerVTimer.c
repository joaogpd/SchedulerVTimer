#include "SchedulerVTimer.h"
// Choose platform for proper timer setup and configuration
#include "../timer_component/MSP430FR59xxTimer.h"
#include "../timer_component/ATMEGA328pTimer.h"

#ifdef __cplusplus
extern "C" {
#endif

task_ptr_t _appTasks[MAX_APP_TASKS]; /**< Array of function pointers to the tasks of type app that were added to the scheduler. */  
volatile uint8_t _appTasksFlag[MAX_APP_TASKS] = {false}; /**< Array of flags that determine whether an app task has been posted for execution or not. */  
volatile uint8_t _appTasksArg[MAX_APP_TASKS] = {0}; /**< Array of arguments for the app tasks by their corresponding position. */  

task_ptr_t _vtTasks[MAX_VT_TASKS]; /**< Array of function pointers to the tasks of type vtimer that were added to the scheduler. */ 
volatile uint8_t _vtTasksFlag[MAX_VT_TASKS] = {false}; /**< Array of flags tha determine whether a vtimer task has been posted for execution or not. */
uint8_t _vtTasksArg[MAX_VT_TASKS] = {0}; /**< Array of arguments for the vtimer tasks by their corresponding position. */  
volatile uint16_t _vtTasksTimer[MAX_VT_TASKS] = {0}; /**< Array of timer counter for the vtimer tasks by their corresponding position, that is, how many times it will run. */  

/**
 * This function adds the tasks passed as arguments to the scheduler, allowing these to
 * be posted by the user to be later executed.
 * @param taskType the type of the tasks that will be added to the scheduler, valid values are app and vtimer
 * @param ... variable number of arguments, name of the tasks that need to be added to the scheduler
 */
void setupTasks(taskId_t taskType,...) {
    va_list args;
    task_ptr_t* auxPtr;
    uint8_t* auxFlag;
    uint8_t* auxArg;
    uint8_t taskCount = 0;
    uint8_t maxTasks;

    if (taskType == app) {
      auxPtr  = _appTasks;
      auxFlag = _appTasksFlag;
      auxArg  = _appTasksArg;
      maxTasks = MAX_APP_TASKS;
    } else {
      auxPtr = _vtTasks;
      auxFlag = _vtTasksFlag;
      auxArg = _vtTasksArg;
      maxTasks = MAX_VT_TASKS;
    }
    va_start(args, taskType);
    auxPtr[taskCount++] = va_arg(args, task_ptr_t);
    while ((auxPtr[taskCount++] = va_arg(args, task_ptr_t)) != 0 && taskCount < maxTasks) {
      auxFlag[taskCount] = false;
      auxArg[taskCount] = 0;
      if (taskType == vtimer) 
        _vtTasksTimer[taskCount] = 0;
    }
    taskCount--;
    va_end(args); 
}

/**
 * This function iterates over the task array, whether vt or app, 
 * to get the numeric id (or the position) of a given task.
 * @param taskType the type of the task to be searched, valid values are app and vtimer
 * @param fx the name of the task to be searched
 */
int8_t getTaskIdx(taskId_t taskType, task_ptr_t fx) {
    task_ptr_t* auxPtr;
    uint8_t maxTasks;
    if (taskType == app) {
      auxPtr  = _appTasks;
      maxTasks = MAX_APP_TASKS;
    } else {
      auxPtr = _vtTasks;
      maxTasks = MAX_VT_TASKS;
    }
    for (uint8_t i = 0; i < maxTasks; i++) {
        if (fx == auxPtr[i]) 
          return i;
    }
    return -1;
}

/**
 * This function enables a given virtual timer task to be executed.
 * @param taskId the id of the task to be posted
 */
void postVTTaskById(uint8_t taskId) {
  _vtTasksFlag[taskId] = true;
}

/**
 * This function posts a task to be executed by its name, altering the [app|vt]TasksFlag array. It also
 * stores the argument that needs to be passed to the task, by the [app|vt]TasksArg array.
 * @param taskType the type of the task to be posted, valid values are app and vtimer
 * @param fx the name of the task to be posted
 * @param arg1 the argument that will be passed to the task to be posted
 */
int8_t postTask(taskId_t taskType, task_ptr_t fx, uint8_t arg1) {
    int8_t idx; 
    uint8_t* auxFlag;
    uint8_t* auxArg;
    auxFlag = (taskType == app) ? _appTasksFlag : _vtTasksFlag ;
    auxArg = (taskType == app) ? _appTasksArg : _vtTasksArg ;
    idx = getTaskIdx(taskType, fx);
    if (idx >= 0) {
        auxArg[idx] = arg1;
        auxFlag[idx] = true;
        return 0;
    } else
        return -1;
}

/**
 * This function changes the _vtTasksTimer array to determine how many timers need to
 * elapse before a given task can be executed. The count is determined by dividing the
 * time in milisseconds that needs to pass by 16. 
 * @param fx the name of the task that will have a timer associated to it
 * @param tmilli the amount of time in milisseconds that needs to pass before the task is executed 
 */
void startVTimer(task_ptr_t fx, uint16_t tmilli, uint8_t arg1) {
    int8_t idx = getTaskIdx(vtimer, fx);
    if (tmilli > 65520) 
      tmilli = 65520;
    if (idx >= 0) {
      _vtTasksTimer[idx] = (uint16_t)(tmilli / 16);
      _vtTasksArg[idx] = arg1;
    }
}

/**
 * This function sets the timer count of a given virtual timer task to zero, so it executes
 * the next time a Timer2 overflow interrupt is generated.
 * @param fx the name of the task that will have its timer count set to zero
 */
void stopVTimer(task_ptr_t fx) {
    int8_t idx = getTaskIdx(vtimer, fx);
    _vtTasksTimer[idx] = (uint16_t)(0); 
}


/**
 * This function performs a loop over the appTasks and vtTasks arrays to find tasks that
 * have been posted and need to be executed. After it executes a task, it marks it to not be 
 * executed again. It assumes that there are no further tasks to be executed when entering the 
 * loop, and changes this only if it finds a task that needs to be executed. Otherwise, it
 * exits the loop altogether, for it has finished executing everything that was expected.
 */
void procTasks(){
    task_ptr_t* auxPtr;
    uint8_t* auxFlag;
    uint8_t* auxArg;
    uint8_t haveTasks=true;
    uint8_t maxTasks;

    // Call pending tasks
    while (haveTasks) {
        haveTasks = false;
        for (uint8_t taskType = app; taskType<=vtimer;taskType++) {
            auxPtr = (taskType == app) ? _appTasks : _vtTasks;
            auxFlag = (taskType == app) ? _appTasksFlag : _vtTasksFlag ;
            auxArg = (taskType == app) ? _appTasksArg : _vtTasksArg ;
            maxTasks = (taskType == app) ? MAX_APP_TASKS : MAX_VT_TASKS ;
            for (uint8_t tt = 0; tt < maxTasks; tt++) {
                if (auxFlag[tt] == true) {
                    auxFlag[tt] = false;
                    haveTasks = true;
                    auxPtr[tt](auxArg[tt]);
                }
            }       
        }
    }
}

/**
 * This function calls startClockTimer to setup Timer2.
 */
void initSchedulerVTTimer() {
  startClockTimer();
}

#ifdef __cplusplus
}
#endif
