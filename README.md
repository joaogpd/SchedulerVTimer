# SchedulerVTimer

This library implements a scheduler for designing a task based application, for any environment using the C language. It was originally made for
Arduino development, but it is generic enough to be used in other environments.

It also implements virtual timers, that can only be used with the Arduino boards, as they depend on the timer registers, which vary from device to device. 
It can be very easily extended, however, to other processors, such as the MSP430, from Texas Instruments. However, at present time this hasn't been implemented whatsover.

TODO: Make different files for each platform, all in one does not seem to work.

