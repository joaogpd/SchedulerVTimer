# SchedulerVTimer

This library, SchedulerVTimer has two main functionalities: a scheduler and a virtual timer interface, for use mainly in microcontroller applications. 

The scheduler portion is largely platform agnostic. Only different compilers may require adaptations of the code, but the changes wouldn't
need to be very substantial. It can also be used outside of a microcontroller scenario, with a regular program running in an ordinary device.
Most important of all, this scheduler allows for task-based applications, a paradigm which seeks to encapsulate code logic in short sections of code, which may be posted to execution by the user, and executed in whichever order is needed. This has exciting implications in intermittent computing as well, and is 
currently one of the leading techniques in ensuring execution progress.

The virtual timer is not platform agnostic whatsoever, and largely depends on what environment it is running. As of now, the timer_component is separate and located in a different folder from the main module itself. Implementations for both ATMega328p and MSP430FR59xx have been done. Both have been tested with the examples located in the examples folder and seems to be working properly. The MSP430 example was compiled using the GNU 9.3.1 tools, and developed on Code Composer Studio 12, while the Arduino example was developed using Arduino IDE version 2.0.4.

New platforms can be added very easily, provided one knows how to configure the Timer for the given architecture.

As previously mentioned, there are a few application examples that make use of this library, and more of those can be created in the future, to further show how to use SchedulerVTimer in an application.

