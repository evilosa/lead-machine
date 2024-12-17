#include <avr/io.h>
#include <lead-machine.h>
#include "millis.h"

int TIME_CHANNEL = 0x00; // PIN ADC0
int SPEED_CHANNEL = 0x01; // PIN ADC1

unsigned int getADCValue(int channel);

static void setup(struct LeadMachine *this) {
  // DDRD &= ~(1 << DDD0);
  // PORTD &= ~(1 << PD0);
  this->settings.mode = MANUAL;
  this->settings.speed = 1;
  this->settings.time = 1000;
  this->state = IDLE;
}

static void updateTime(struct LeadMachine *this) {
  this->settings.time = getADCValue(TIME_CHANNEL) * 5;
}

static void updateSpeed(struct LeadMachine *this) {
  this->settings.speed = getADCValue(SPEED_CHANNEL);
}

static void updateMode(struct LeadMachine *this) {
  if (((PIND & (1 << PD0)) != 0)) {
    this->settings.mode = AUTOMATIC;
  } else {
    this->settings.mode = MANUAL;
  }
}

static void run(struct LeadMachine *this) {
  this->startedAt = millis();
  this->state = FEED_LEAD;
}

static void stop(struct LeadMachine *this) {
  this->state = IDLE;
}

static void reverse(struct LeadMachine *this) {
  this->state = REVERSE_FEED;
}

static struct LeadMachine new() {
  return (struct LeadMachine) {
    .settings = {
      .mode = AUTOMATIC,
      .speed = 0,
      .time = 0
    },
    .state = INIT,
    .startedAt = 0,
    .setup = &setup,
    .updateTime = &updateTime,
    .updateSpeed = &updateSpeed,
    .updateMode = &updateMode,
    .run = &run,
    .stop = &stop,
    .reverse = &reverse
  };
}

const struct LeadMachineClass LeadMachine={.new = &new};

unsigned int getADCValue(int channel)
{
  // ADEN: Set 1 to turn on ADC
  // ADSC: Set 1 for single measure
  // ADPS2 and ADPS1 set to 1 make division factor equal 64
  ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADPS2) | (1 << ADPS1);
  ADMUX = channel; // set channel for measurement
  ADCSRA |= (1 << ADSC); // ADSC will be set to 0 when measurement is completed
  while (ADCSRA & (1 << ADSC));

  return ADCW; // ADCW is a union of ADCH and ADCL registers
}
