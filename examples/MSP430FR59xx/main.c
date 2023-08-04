/**
 * This example is a simple program that turns on and off the P1.0 led in the MSP430FR5994 development
 * board every 5000 milisseconds, or 5 seconds. The led toggle function is subject to a virtual timer
 * and the rest of the configuration is standard for the board.
 */
#include <msp430.h>
#include "SchedulerVTimer.h"

#define UNUSED 0
#define timerLength 5000 // in ms

void blink_led(uint8_t unused);

/**
 * main.c
 */
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	// Configure GPIO P1.0 (led pin)
	P1DIR |= BIT0;
    P1OUT |= BIT0;

    // Enable global interrupts, sets GIE bit on SR
    __bis_SR_register(GIE);

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Setups blink_led as a virtual timer task, and initializes the timer
    setupTasks(vtimer, blink_led);
    initSchedulerVTTimer();
    startVTimer(blink_led, timerLength);

    // Infinite loop always looking for tasks to execute
    while (1)
        procTasks();
}

// Toggles the P1.0 led and restarts the virtual timer associated with the task
void blink_led(uint8_t unused) {
    P1OUT ^= BIT0;
    startVTimer(blink_led, timerLength);
}

