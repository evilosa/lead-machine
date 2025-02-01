#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include <util/twi.h>
#include <stdint.h>

#include "micros.h"

volatile unsigned long int systemMillis = 0;
volatile unsigned long int systemMilliFractional = 0;
volatile unsigned long int systemMonotonicOverflowCnt = 0;

ISR(TIMER0_OVF_vect) {
    unsigned long int m, f;

    m = systemMillis;
    f = systemMilliFractional;

    // Increment the internal current millisecond counter by SYSCLK_TIMER_OVERFLOW_MICROS
    m = m + SYSCLK_MILLI_INCREMENT; 

    // Increment the internal fractional microsecond counter by SYSCLK_MILLIFRACT_INCREMENT
    f = f + SYSCLK_MILLIFRACT_INCREMENT;

    // Whenever the fractional microsecond counter passes over SYSCLK_MILLIFRACT_MAXIMUM 
    // the internal millisecond counter will be incremented by one and the used microseconds 
    // will be removed from the internal microsecond counter.
    if (f > SYSCLK_MILLIFRACT_MAXIMUM) {
        f = f - SYSCLK_MILLIFRACT_MAXIMUM;
        m = m + 1;
    }

    // An additional internal tick counter systemMonotonicOverflowCnt will be incremented on every timer invocation.
    systemMonotonicOverflowCnt = systemMonotonicOverflowCnt + 1;

    systemMillis = m;
    systemMilliFractional = f;
}

void systickInit() {
    uint8_t sregOld = SREG;

    cli();

    TCCR0 |= (1 << CS01); // Устанавливаем предделитель на 8 CS2=0, CS1=1, CS0=0
    TIMSK |= (1 << TOIE0); // разрешаем прерывание по переполнению

    SREG = sregOld;
}

void systickDisable() {
	TIMSK = 0x00;
}

unsigned long int millisN() {
    unsigned long int m;

    uint8_t srOld = SREG;
    cli();

    m = systemMillis;
    SREG = srOld;

    return m;
}

unsigned long int microsN() {
    uint8_t srOld = SREG;

    unsigned long int overflowCounter;
    unsigned long int timerCounter;

    cli();
    overflowCounter = systemMonotonicOverflowCnt;
    timerCounter = TCNT0;

    if (((TIFR & 0x01) != 0) && (timerCounter < 255)) {
        overflowCounter = overflowCounter + 1;
    }

    SREG = srOld;

    return ((overflowCounter << 8) + timerCounter) * (8L / (F_CPU / 1000000L));
}

void delayN(unsigned long millisecs) {
    unsigned int lastMicro;

    lastMicro = (unsigned int)microsN();

    while (millisecs > 0)
    {
        unsigned int curMicro = microsN();
        if (curMicro - lastMicro >= 1000) {
            lastMicro = lastMicro + 1000;
            millisecs = millisecs - 1;
        }
    }
    return;
}

void delayMicrosN(unsigned int microDelay) {
	#if F_CPU == 20000000L
		__asm__ __volatile__ (
			"nop\n"
			"nop\n"
		);
		if((microDelay = microDelay - 1) == 0) {
			return;
		}

		microDelay = (microDelay << 2) + microDelay;
	#elif F_CPU == 16000000L
		if((microDelay = microDelay - 1) == 0) {
			return;
		}

		microDelay = (microDelay << 2) - 2;
	#elif F_CPU == 8000000L
		if((microDelay = microDelay - 1) == 0) {
			return;
		}
		if((microDelay = microDelay - 1) == 0) {
			return;
		}

		microDelay = (microDelay << 1) - 1;
	#else
		#error No known delay loop calibration available for this F_CPU
	#endif

	__asm__ __volatile__ (
		"lp: sbiw %0, 1\n"
		"    brne lp"
		: "=w" (microDelay)
		: "0" (microDelay)
	);
	return;
}