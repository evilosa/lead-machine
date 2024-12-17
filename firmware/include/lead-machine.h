#ifndef LeadMachine_h
#define LeadMachine_h

enum Mode {
  AUTOMATIC,
  MANUAL
};

enum State {
  INIT, // machine initialization
  IDLE, // machine awaiting user commands
  FEED_LEAD,
  REVERSE_FEED
};

struct Settings {
  enum Mode mode;
  unsigned int time;
  unsigned int speed;
};

struct LeadMachine {
  struct Settings settings;

  enum State state;

  unsigned long startedAt;

  void (*setup)(struct LeadMachine *this);
  void (*updateTime)(struct LeadMachine *this);
  void (*updateSpeed)(struct LeadMachine *this);
  void (*updateMode)(struct LeadMachine *this);

  void (*run)(struct LeadMachine *this);
  void (*stop)(struct LeadMachine *this);
  void (*reverse)(struct LeadMachine *this);
};

extern const struct LeadMachineClass {
  struct LeadMachine (*new)();
} LeadMachine;

#endif