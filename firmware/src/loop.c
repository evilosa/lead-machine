#include <avr/io.h>
#include <util/delay.h>
#include "button.h"
#include "lead-machine.h"
#include "millis.h"

#ifndef _BV
#define _BV(BIT) (1 << (BIT))
#endif
#define PORT_ON(port, pin) port |= (1 << pin)
#define PORT_OFF(port, pin) port &= ~(1 << pin)

struct Button reverseButton; // PD1
struct Button feedButton;    // PD2

struct LeadMachine machine;

void handleFeedPressed();
void handleFeedReleased();
void handleReversePressed();
void handleReverseReleased();

void setup(void)
{
  millis_init();

  // Настраиваем порты вывода
  DDRD |= (1 << DDD7);
  PORTD = PORTD | _BV(PD7);

  // Настраиваем порты вывода шагового двигателя
  DDRC |= (1 << DDC5) | (1 << DDC4) | (1 << DDC3) | (1 << DDC2);
  PORTC &= ~(1 << PC5);
  PORTC &= ~(1 << PC4);
  PORTC &= ~(1 << PC3);
  PORTC &= ~(1 << PC2);

  // Подключаем кнопки
  feedButton = Button.new(&PIND, PD2);
  feedButton.onButtonPressed = &handleFeedPressed;
  feedButton.onButtonReleased = &handleFeedReleased;

  reverseButton = Button.new(&PIND, PD1);
  reverseButton.onButtonPressed = &handleReversePressed;
  reverseButton.onButtonReleased = &handleReverseReleased;

  // Инициализируем машину
  machine = LeadMachine.new();
  machine.setup(&machine);

  // Blink with LED for tests
  _delay_ms(500);
  PORTD &= ~(1 << PD7);
}

void loop(void)
{
  setup();

  for (;;)
  {
    asm("NOP");

    feedButton.update(&feedButton);
    reverseButton.update(&reverseButton);

    switch (machine.state)
    {
      case IDLE:
        PORT_OFF(PORTC, PC5);
        machine.updateMode(&machine);
        machine.updateTime(&machine);
        machine.updateSpeed(&machine);

        if (machine.settings.mode == AUTOMATIC) {
          PORT_ON(PORTC, PC3);
          _delay_ms(30);
          PORT_OFF(PORTC, PC3);
        } else {
          PORT_ON(PORTC, PC2);
          _delay_ms(30);
          PORT_OFF(PORTC, PC2);
        }
      
      case FEED_LEAD:
        PORT_ON(PORTC, PC5);
        if ((machine.settings.mode == AUTOMATIC) & (millis() - machine.startedAt > machine.settings.time)) {
          machine.stop(&machine);
        }
        break;

      case REVERSE_FEED:
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
  if ((machine.settings.mode = MANUAL)) {
    machine.stop(&machine);
  }
}

void handleReversePressed() {
  machine.reverse(&machine);
}

void handleReverseReleased() {
  machine.stop(&machine);
}