#ifndef StepperEngine_h
#define StepperEngine_h

struct StepperEngine {
	void (*setup)(struct StepperEngine *this);
  void (*setSpeed)(struct StepperEngine *this, int speed_us);
  void (*step)(struct StepperEngine *this);
	void (*stop)(struct StepperEngine *this);
};

extern const struct StepperEngineClass {
  struct StepperEngine (*new)();
} StepperEngine;

#endif