#ifndef __MILLIS___
#define __MILLIS___

#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#define CTC_MATCH_OVERFLOW 124

extern volatile uint32_t timer1_millis;

ISR                   (TIMER2_COMP_vect);
uint32_t  millis      (void);
void      millis_init (void);

#endif