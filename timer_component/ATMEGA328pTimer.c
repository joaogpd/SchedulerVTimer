#include "ATMEGA328pTimer.h"

void postVTTaskById(uint8_t taskId);

#ifdef __cplusplus
extern "C" {
#endif

extern volatile uint16_t _vtTasksTimer[MAX_VT_TASKS]; /**< Array of timer counter for the vtimer tasks by their corresponding position, that is, how many times it will run. */  

/**
 * This function setups the registers on the ATMega328p to start 
 * Timer 2 with the given parameters, and enables interrupt on timer overflow.
 */
void startClockTimer() {
  /*
   * TCNT2 = 255 – t*(clk/1024)
   * TCNT2 = 255 – 0,30*(16000000/1024) = 46785 // 0.30s
   */
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = (uint8_t)(255 - (0.016 * 16000000 / 1024)); // preload timer 
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20) ;          // 1024 prescaler
  TIMSK2 |= (1 << TOIE2);               // enable timer overflow interrupt ISR
}

/**
 * This Timer2 Interrupt Service Routine is triggered whenever a Timer2 expires. 
 * It decrements the value of a position of _vtTasksTimer it finds to be higher than 0, 
 * to count down on the amount of times it needs to be executed further.
 * It also restores the timer value to the starting one, allowing the timer to run again.
 */
ISR(TIMER2_OVF_vect) {
  TCNT2 = (uint8_t)(255 - (0.016 * 16000000 / 1024)); // preload timer 
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
