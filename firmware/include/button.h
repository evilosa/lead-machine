#ifndef Button_h
#define Button_h

#include <stdbool.h>
#include <stdint.h>

struct Button {
  volatile uint8_t* _port;
  int _pin;
  bool _isReleased;
  int _prevReleasedState;
  uint32_t _lastPressedAt;
  uint32_t _debounceDelayMs;

  void (*update)(struct Button *this);
  void (*onButtonPressed)();
  void (*onButtonReleased)();
};

extern const struct ButtonClass {
  struct Button (*new)(volatile uint8_t* port, int pin);
} Button;


#endif