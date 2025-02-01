// See instructions on a next page
// https://www.tspi.at/2021/07/26/simplesysclockavr.html

#ifndef __MICROS___
#define __MICROS___

#define SYSCLK_TIMER_OVERFLOW_MICROS (8L * ((256L * 1000000L) / F_CPU))
#define SYSCLK_MILLI_INCREMENT (SYSCLK_TIMER_OVERFLOW_MICROS / 1000L)
#define SYSCLK_MILLIFRACT_INCREMENT ((SYSCLK_TIMER_OVERFLOW_MICROS % 1000L) >> 3)
#define SYSCLK_MILLIFRACT_MAXIMUM (1000 >> 3)

void systickInit();
void systeckDisable();

unsigned long int millisN();
unsigned long int microsN();

void delayN(unsigned long millisecs);
void delayMicrosN(unsigned int microDelay);

#endif