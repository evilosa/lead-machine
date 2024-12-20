#include <avr/io.h>
#include <stepper-engine.h>
#include "helpers.h"
#include "millis.h"

int speedUs = 2000;
int stepIndex = 0;
int stopStep[4] = {0, 0, 0, 0};
int steps[4][4] = {
  {1, 0, 0, 1},
  {0, 0, 1, 1},
  {0, 1, 1, 0},
  {1, 0, 0, 0}
};

static void step(int stepValue[4]);

static void setup(struct StepperEngine *this) {
  // Настраиваем порты вывода шагового двигателя
  DDRC |= (1 << DDC5) | (1 << DDC4) | (1 << DDC3) | (1 << DDC2);

  this->stop(this);
  stepIndex = 0;
  speedUs = 2000;
}

static void setSpeed(struct StepperEngine *this, int measuredSpeedUs) {
  speedUs = 2000 + (1024 - measuredSpeedUs );
}

//////////////////////
// Nema17 contacts
// A - Black 
// B - Red    
// C - Green  
// D - Blue   

//////////////////////
// A // B // C // D //
// 1 // 1 // 0 // 0 //
// 0 // 1 // 1 // 0 //
// 0 // 0 // 1 // 1 //
// 1 // 0 // 0 // 1 //
//////////////////////

static void stepForward(struct StepperEngine *this) {
  step(steps[stepIndex]);
  stepIndex++;
  if (stepIndex > 3) stepIndex = 0;
  _delay_ms(2);
}

static void stepBackward(struct StepperEngine *this) {
  step(steps[stepIndex]);
  stepIndex--;
  if (stepIndex < 0) stepIndex = 3;
  _delay_ms(2);
}

static void stop(struct StepperEngine *this) {
  step(stopStep);
}



static void step(int stepValues[4]) {
  if (stepValues[0]) {
    PORT_ON(PORTC, PC5);
  } else {
    PORT_OFF(PORTC, PC5);
  }

  if (stepValues[1]) {
    PORT_ON(PORTC, PC4);
  } else {
    PORT_OFF(PORTC, PC4);
  }

  if (stepValues[2]) {
    PORT_ON(PORTC, PC3);
  } else {
    PORT_OFF(PORTC, PC3);
  }

  if (stepValues[3]) {
    PORT_ON(PORTC, PC2);
  } else {
    PORT_OFF(PORTC, PC2);
  }
}

static struct StepperEngine new() {
  return (struct StepperEngine) {
    .setup = &setup,
    .setSpeed = &setSpeed,
    .stepForward = &stepForward,
    .stepBackward = &stepBackward,
    .stop = &stop
  };
}

const struct StepperEngineClass StepperEngine={.new = &new};
