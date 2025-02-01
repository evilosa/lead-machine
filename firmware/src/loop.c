#include <avr/io.h>
#include <util/delay.h>
#include "button.h"
#include "lead-machine.h"
#include "stepper-engine.h"
#include "helpers.h"
#include "millis.h"
#include "micros.h"

struct Button reverseButton; // PD1
struct Button feedButton;    // PD2

struct LeadMachine machine;
struct StepperEngine engine;

int ledOn;
unsigned long ledOnAt = 0;

void handleFeedPressed();
void handleFeedReleased();
void handleReversePressed();
void handleReverseReleased();

void setup(void)
{
  millis_init();
  systickInit();

  // Настраиваем порты вывода
  DDRC |= (1 << DDC3);
  PORTC = PORTC | _BV(PC3);

  // Подключаем кнопки
  feedButton = Button.new(&PIND, PD2);
  feedButton.onButtonPressed = &handleFeedPressed;
  feedButton.onButtonReleased = &handleFeedReleased;

  reverseButton = Button.new(&PIND, PD1);
  reverseButton.onButtonPressed = &handleReversePressed;
  reverseButton.onButtonReleased = &handleReverseReleased;

  engine = StepperEngine.new();
  engine.setup(&engine);

  // Инициализируем машину
  machine = LeadMachine.new();
  machine.setup(&machine);

  // Blink with LED for tests
  _delay_ms(500);
  // PORTC &= ~(1 << PC3);
  PORT_OFF(PORTC, PC3);
  ledOn = 0;
}

void loop(void)
{
  setup();

  for (;;)
  {
    blinkTimerLed();
    // asm("NOP");

    feedButton.update(&feedButton);
    reverseButton.update(&reverseButton);
    machine.updateSpeed(&machine);
    engine.setSpeed(&engine, machine.settings.speed);

    switch (machine.state)
    {
      case IDLE:
        engine.stop(&engine);
        machine.updateMode(&machine);
        machine.updateTime(&machine);
        
        break;

      
      case FEED_LEAD:
        engine.step(&engine);
        if ((machine.settings.mode == AUTOMATIC) & (millis() - machine.startedAt > machine.settings.time)) {
          machine.stop(&machine);
        }
        break;

      case REVERSE_FEED:
        engine.step(&engine);
        break;

      default:
        break;
    }
    
  }
}

void handleFeedPressed() {
  machine.run(&machine);
}

void handleFeedReleased() {
  if ((machine.settings.mode == MANUAL)) {
    machine.stop(&machine);
  }
}

void handleReversePressed() {
  machine.reverse(&machine);
}

void handleReverseReleased() {
  machine.stop(&machine);
}

void blinkTimerLed() {
  if (microsN() - 1000000 > ledOnAt) {
    ledOnAt = microsN();
    if (ledOn == 1) {
      PORT_OFF(PORTC, PC3);
      ledOn = 0;
    } else {
      PORT_ON(PORTC, PC3);
      ledOn = 1;
    }
  }
}