#include <avr/io.h>
#include <stepper-engine.h>
#include "helpers.h"
#include "millis.h"
#include "micros.h"

int minSpeed = 1;
int speedMs = 1;
int speedUs = 1000;
uint32_t startedAt = 0;
unsigned long startedAtUs = 0;

enum State {
  ON,
  OFF
};

enum State prevState;
static int isDelayPassed();

static void setup(struct StepperEngine *this) {
  prevState = OFF;

  // Настраиваем порты вывода шагового двигателя
  DDRC |= (1 << DDC5) | (1 << DDC4);


  // Настраиваем порт вывода Enable
  DDRD |= (1 << DDD3);

  // Настраиваем порты вывода MS1-3
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);

  PORT_OFF(PORTD, PD3);

  PORT_ON(PORTB, PB0);
  PORT_OFF(PORTB, PB1);
  PORT_OFF(PORTB, PB2);

  PORT_ON(PORTC, PC4);

  this->stop(this);
  speedMs = 1;
  speedUs = 1000;
}

// PD3 - на Enable
// PB0 - на MS1
// PB1 - на MS2
// PB2 - на MS3


// Комбинация значений для выбора микрошага
// MS1	MS1	 MS1	Дробление шага
//  0	   0	  0	     1
//  1	   0	  0	     1/2
//  0	   1	  0	     1/4
//  1	   1	  0	     1/8
//  1	   1	  1	     1/16

static void setSpeed(struct StepperEngine *this, int measuredSpeedUs) {
  speedMs = 1 + (1024 - measuredSpeedUs ) / 100;
  speedUs = 150 + (1024 - measuredSpeedUs);
}

static void step(struct StepperEngine *this) {
  if (isDelayPassed() == 0) return;

  if (prevState == ON) {
    prevState = OFF;
    PORT_OFF(PORTC, PC5);
  } else {
    prevState = ON;
    PORT_ON(PORTC, PC5);    
  }
}

static void stop(struct StepperEngine *this) {
  PORT_OFF(PORTC, PC5);
}

static struct StepperEngine new() {
  return (struct StepperEngine) {
    .setup = &setup,
    .setSpeed = &setSpeed,
    .step = &step,
    .stop = &stop
  };
}

static int isDelayPassed() {
  // if (millis() - speedMs > startedAt) {
  //   startedAt = millis();
  //   return 1;
  // }

  if (microsN() - speedUs > startedAtUs) {
    startedAtUs = microsN();
    return 1;
  }

  return 0;
}
const struct StepperEngineClass StepperEngine={.new = &new};
