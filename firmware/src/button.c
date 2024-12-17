#include <avr/io.h>
#include <util/delay.h>
#include "button.h"
#include "millis.h"

static void update(struct Button *this) {
  // считываем текущее состояние нажатия
  int isReleased = ((*(this->_port) & (1<<this->_pin)) != 0);

  // сравниваем с прошлым состоянием кнопки
  if (isReleased != this->_prevReleasedState) {
    // Если состояние нажатия поменялось то сохраняем время смены состояния
    this->_lastPressedAt = millis();
  }

  // Указанная задержка прошла
  if ((millis() - this->_lastPressedAt) > this->_debounceDelayMs) {
    if (isReleased != this->_isReleased) { // состояние нажатия кнопки изменилось
      if (isReleased) {
        this->onButtonReleased(); // кнопка была отпущена
      } else {
        this->onButtonPressed(); // кнопка была нажата
      }

      this->_isReleased = isReleased; // сохраним новое состояние нажатия кнопки
    }
  }

  this->_prevReleasedState = isReleased;
}

static struct Button new(volatile uint8_t* port, int pin) {
  return (struct Button){
    ._port = port,
    ._pin = pin,
    ._isReleased = true,
    ._prevReleasedState = 1,
    ._lastPressedAt = millis(),
    ._debounceDelayMs = 60,
    .update = &update
  };
}

const struct ButtonClass Button={.new = &new};

