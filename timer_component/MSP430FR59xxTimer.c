#include "MSP430FR59xxTimer.h"

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint16_t _vtTasksTimer[MAX_VT_TASKS]; /**< Array of timer counter for the vtimer tasks by their corresponding position, that is, how many times it will run. */  

/**
 * This function setups the registers on the MSP430FR59xx to start
 * Timer A with the given parameters, and enables capture/compare interrupt 
 */
void startClockTimer() {
  TA0CCTL0 = CCIE; // TACCR0 interrupt enabled
  TA0CCR0 = (uint8_t)(255 - (0.016 * 16000000 / 1024)); // preload timer
  TA0CTL = TASSEL__SMCLK | MC__CONTINOUS; // SMCLK, continous mode
}

/**
 * This TimerA Interrupt Service Routine is triggered whenever a TimerA expires. 
 * It decrements the value of a position of _vtTasksTimer it finds to be higher than 0, 
 * to count down on the amount of times it needs to be executed further.
 * It also restores the timer value to the starting one, allowing the timer to run again.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_AO_ISR(void) 
#elif defined(__GNUC__)
void __attribute__  ((interrupt(TIMER0_A0_VECTOR))) Timer0_AO_ISR(void)
#else
#error Compiler not supported!
#endif
{
  TA0CCR0 = (uint8_t)(255 - (0.016 * 16000000 / 1024)); // preload timer 
  for (uint8_t i = 0; i < MAX_VT_TASKS; i++){
    if (_vtTasksTimer[i] > 0) {
      _vtTasksTimer[i]--;
      if (_vtTasksTimer[i] <= 0) {
        _vtTasksTimer[i] = 0;
        postVTTaskById(i);
      }
    }
  }
}

#ifdef __cplusplus
}
#endif
