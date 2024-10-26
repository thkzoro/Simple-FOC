#ifndef __GPIO_H
#define __GPIO_H
#include <gpiod.h>
#include <cstdint>

#define RISING    GPIOD_LINE_EDGE_RISING
#define FALLING   GPIOD_LINE_EDGE_FALLING
#define CHANGE    GPIOD_LINE_EDGE_BOTH

#define HIGH      1
#define LOW       0
#define INPUT     GPIOD_LINE_DIRECTION_INPUT
#define OUTPUT    GPIOD_LINE_DIRECTION_OUTPUT
#define INPUT_PULLUP GPIOD_LINE_DIRECTION_INPUT



void pinMode(unsigned int pin, int mode);
int digitalRead(unsigned int pin);
void digitalWrite(unsigned int pin, int value);
int analogRead(unsigned int pin);
void analogWrite(unsigned int pin, int value);

#endif // __GPIO_H