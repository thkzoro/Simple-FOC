#ifndef __ARDUINO_H
#define __ARDUINO_H

#include <stdint.h>
#include <math.h>
#include <algorithm>
#include <cctype>

#include "wiringPi.h"
#include "print.h"
#include "stream.h"
#include "Astring.h"




#define isDigit isdigit
//#define	INT_EDGE_SETUP
//#define	INT_EDGE_FALLING
#define	RISING  INT_EDGE_RISING
#define	CHANGE  INT_EDGE_BOTH

#define	INPUT_PULLUP INPUT
//#define	OUTPUT

using word = uint16_t;
using byte = uint8_t;

template<typename T>
inline T min(T a, T b) { return std::min(a, b); }
template<typename T>
inline T max(T a, T b) { return std::max(a, b); }

int digitalPinToInterrupt(int pin);

void attachInterrupt(int pin, void (*ISR)(void), int mode);

void noInterrupts() {
}

void interrupts() {
}

unsigned long pulseIn(int pin, int value, unsigned long timeout);
#endif //__ARDUINO_H
