#ifndef __GPIO_H
#define __GPIO_H
#include <cstdint>
#include <gpiod.hpp>


constexpr int RISING = static_cast<int>(gpiod::line::edge::RISING);
constexpr int FALLING = static_cast<int>(gpiod::line::edge::FALLING);
constexpr int CHANGE = static_cast<int>(gpiod::line::edge::BOTH);

constexpr int INPUT = static_cast<int>(gpiod::line::direction::INPUT);
constexpr int OUTPUT = static_cast<int>(gpiod::line::direction::OUTPUT);

constexpr int INPUT_PULLUP  = static_cast<int>(gpiod::line::bias::PULL_UP);
constexpr int INPUT_PULLDOWN = static_cast<int>(gpiod::line::bias::PULL_DOWN);

constexpr int LOW = static_cast<int>(gpiod::line::value::INACTIVE);
constexpr int HIGH = static_cast<int>(gpiod::line::value::ACTIVE);

void pinMode(unsigned int pin, int mode);
int digitalRead(unsigned int pin);
void digitalWrite(unsigned int pin, int value);
int analogRead(unsigned int pin);

#endif // __GPIO_H