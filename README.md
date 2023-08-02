# SchedulerVTimer

This library, SchedulerVTimer has two main functionalities: a scheduler and a virtual timer interface, for use mainly in microcontroller applications. 

The scheduler portion is largely platform agnostic. Only different compilers may require adaptations of the code, but the changes wouldn't
need to be very substantial. It can also be used outside of a microcontroller scenario, with a regular program running in an ordinary device.
Most important of all, this scheduler allows for task-based applications, a paradigm which seeks to encapsulate code logic in short sections of code, which may be posted to execution by the user, and executed in whichever order is needed. This has exciting implications in intermittent computing as well, and is 
currently one of the leading ways of ensuring execution progress when employing this paradigm.

The virtual timer is not platform agnostic whatsoever, and largely depends on what environment it is running. As of now, the SchedulerVTimer library supports the ATMega328p and the MSP430FR59xx platforms, each of those in a different folder. The former has been tested with the example located in the examples folder and seems to be working properly, while the latter hasn't been tested, and may not be working at all.

New platforms can be added very easily, provided one knows how to configure the Timer for the given architecture.

As previously mentioned, there are a few (as of now only one) application examples that make use of this library, and more of those can be created in the future, to further show how to use SchedulerVTimer in an application.

This branch, the original, is not the ideal way of implementing this library, and timer_component is a better tell of how things should be done to improve
the project quality.

