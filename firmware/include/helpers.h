#ifndef Helpers_h
#define Helpers_h

#define BV(BIT) (1 << (BIT))
#define PORT_ON(port, pin) port |= (1 << pin)
#define PORT_OFF(port, pin) port &= ~(1 << pin)

#endif