#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <arduino.h>
#include <stdarg.h>
#include <iostream>

#ifndef MAX_APP_TASKS
#define MAX_APP_TASKS 10
#endif

// #ifndef MAX_VT_TASKS
// #define MAX_VT_TASKS 5
// #endif

class Scheduler {
protected:
    typedef enum {
        app = 0,
        vtimer
    } taskId_t;

    typedef void (*task_ptr_t)(uint8_t);

private:
    std::array<task_ptr_t, MAX_APP_TASKS> _appTasks; /**< Array of function pointers to the tasks of type app that were added to the scheduler. */  
    std::array<volatile uint8_t, MAX_APP_TASKS> _appTasksFlag; /**< Array of flags that determine whether an app task has been posted for execution or not. */ 
    std::array<volatile uint8_t, MAX_APP_TASKS> _appTasksArg; /**< Array of arguments for the app tasks by their corresponding position. */  
    
public:
    Scheduler() {
        _appTasksFlag = {false};
        _appTasksArg = {0};
    }

    

};

#endif