#include "GPIO.h"
#include <stdexcept>
#include <gpiod.hpp>
#include <unordered_map>
#include <memory>
#include <filesystem>
#include "PWM.h"

namespace {
    const std::filesystem::path chip_path("/dev/gpiochip4");
}
class GPIO;
class GPIOManager {
public:
    static GPIOManager& getInstance() {
        static GPIOManager instance;
        return instance;
    }

    ~GPIOManager() {
        m_gpios.clear();
    }

    void addGPIO(int pin, std::shared_ptr<GPIO> &gpio) {
        m_gpios[pin] = gpio;
    }

    std::shared_ptr<GPIO> getGPIO(int pin) {
        auto it = m_gpios.find(pin);
        if (it == m_gpios.end()) {
            return nullptr;
        }
        return it->second;
    }

    gpiod::chip &getChip() {
        return m_chip;
    }

private:
    GPIOManager() : m_chip(chip_path) {}

    GPIOManager(const GPIOManager&) = delete;
    GPIOManager& operator=(const GPIOManager&) = delete;

    gpiod::chip m_chip;
    std::unordered_map<int, std::shared_ptr<GPIO>> m_gpios;
};

class GPIO {
public:
    GPIO(int pin, int mode) :
        m_pin(pin), m_mode(mode) {
        auto builder = GPIOManager::getInstance().getChip().prepare_request();
        builder.set_consumer("gpio_manager");
        builder.add_line_settings(m_pin, gpiod::line_settings().set_direction(
                m_mode == INPUT ?gpiod::line::direction::INPUT : gpiod::line::direction::OUTPUT));
         auto request = builder.do_request();
        m_request = std::make_shared<gpiod::line_request>(std::move(request));
    }

    ~GPIO() {
        m_request->release();
    }

    int getPin() const {
        return m_pin;
    }

    void setValue(int value) {
        if (m_mode != OUTPUT) {
            throw std::runtime_error("Cannot set value on input GPIO");
        }
        m_request->set_value(m_pin, value == HIGH ? gpiod::line::value::ACTIVE : gpiod::line::value::INACTIVE);
    }

private:
    int m_pin;
    int m_mode;
    std::shared_ptr<gpiod::line_request> m_request;
};

void pinMode(unsigned int pin, int mode) {
    if (pin == 22) {
        if (mode == INPUT || mode == OUTPUT) {
            auto gpio =GPIOManager::getInstance().getGPIO(pin);
            if (gpio == nullptr) {
                gpio = std::make_shared<GPIO>(pin, mode);
                GPIOManager::getInstance().addGPIO(pin, gpio);
            }
        }
    } else if (pin == 0 || pin == 1 || pin == 7) {
        if (mode == OUTPUT) {
            auto pwm = PWMManager::getInstance().getPWM(pin);
            if (pwm == nullptr) {
                pwm = std::make_shared<PWM>(pin);
                PWMManager::getInstance().addPWM(pin, pwm);
            }
        }
    }
}

int digitalRead(unsigned int pin) {
    return 0;
}

void digitalWrite(unsigned int pin, int value) {
    auto gpio = GPIOManager::getInstance().getGPIO(pin);
    if (gpio == nullptr) {
        throw std::runtime_error("GPIO not initialized");
    }
    gpio->setValue(value);
}

int analogRead(unsigned int pin) {
    return 0;
}


