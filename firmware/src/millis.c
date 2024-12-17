#include <avr/interrupt.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "millis.h"

volatile uint32_t timer1_millis;

////////////////////////////////////////////////////////////////////////////////
ISR (TIMER2_COMP_vect) {
  timer1_millis++;
}

// Возвращает количество миллисекунд прошедших с начала запуска контроллера
uint32_t millis(void) {
  uint32_t millis_return;

  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    millis_return = timer1_millis;
  }

  return millis_return;
}

////////////////////////////////////////////////////////////////////////////////
void millis_init(void)
{
  TCCR2 |= (1 << WGM21) | (1 << CS20) | (1 << CS21); // режим CTC, тактировать с делителем 64
  OCR2   = (CTC_MATCH_OVERFLOW);
  TIMSK |= (1<<OCIE2);         // Разрешить прерывание по совпадению
}