#ifndef _IO_H_
#define _IO_H_

/* General digital IO functions, simillar to Arduino. */

#include <stdbool.h>
#include <stdint.h>

#define OUTPUT 0
#define INPUT 1
#define INPUT_PULLUP 2

#define IO_PINB0 0
#define IO_PINB1 1
#define IO_PINB2 2
#define IO_PINB3 3
#define IO_PINB4 4
#define IO_PINB5 5
#define IO_PINB6 6
#define IO_PINB7 7

#define IO_PINC0 8
#define IO_PINC1 9
#define IO_PINC2 10
#define IO_PINC3 11
#define IO_PINC4 12
#define IO_PINC5 13
#define IO_PINC6 14
#define IO_PINC7 15

#define IO_PIND0 16
#define IO_PIND1 17
#define IO_PIND2 18
#define IO_PIND3 19
#define IO_PIND4 20
#define IO_PIND5 21
#define IO_PIND6 22
#define IO_PIND7 23

void set_output(uint8_t pin, bool state);
bool get_input(uint8_t pin);
void pin_mode(uint8_t pin, uint8_t mode);

#endif
