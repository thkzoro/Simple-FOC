#include "GPIO.h"
#include <stdexcept>
#include <iostream>



static struct gpiod_chip *chip = nullptr;
static struct gpiod_line_config *line_config = nullptr;
static struct gpiod_line_request *line_request = nullptr;

void initialize() {
    if (!chip) {
        chip = gpiod_chip_open("/dev/gpiochip0");
        if (!chip) {
            throw std::runtime_error("Failed to open GPIO chip");
        }
    }
    if (!line_config) {
        line_config = gpiod_line_config_new();
        if (!line_config) {
            throw std::runtime_error("Failed to create line config");
        }
    }
}

void pinMode(unsigned int pin, int mode) {
    initialize();

    struct gpiod_line_settings *settings = gpiod_line_settings_new();
    if (!settings) {
        throw std::runtime_error("Failed to create line settings");
    }

    gpiod_line_settings_set_direction(settings,
                                      mode == OUTPUT ? GPIOD_LINE_DIRECTION_OUTPUT : GPIOD_LINE_DIRECTION_INPUT);

    if (mode == INPUT_PULLUP) {
        gpiod_line_settings_set_bias(settings, GPIOD_LINE_BIAS_PULL_UP);
    }

    gpiod_line_config_add_line_settings(line_config, &pin, 1, settings);

    gpiod_line_settings_free(settings);

    if (line_request) {
        gpiod_line_request_release(line_request);
    }

    line_request = gpiod_chip_request_lines(chip, NULL, line_config);
    if (!line_request) {
        throw std::runtime_error("Failed to request GPIO lines");
    }
}

int digitalRead(unsigned int pin) {
    if (!line_request) {
        throw std::runtime_error("GPIO lines not requested");
    }

    enum gpiod_line_value value = gpiod_line_request_get_value(line_request, pin);
    if (value == GPIOD_LINE_VALUE_ERROR) {
        throw std::runtime_error("Failed to read GPIO value");
    }

    return value == GPIOD_LINE_VALUE_ACTIVE ? HIGH : LOW;
}

void digitalWrite(unsigned int pin, int value) {
    if (!line_request) {
        throw std::runtime_error("GPIO lines not requested");
    }

    enum gpiod_line_value gvalue = value == HIGH ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;
    if (gpiod_line_request_set_value(line_request, pin, gvalue) < 0) {
        throw std::runtime_error("Failed to set GPIO value");
    }
}

int analogRead(unsigned int pin) {
    std::cout << "analogRead is not implemented for this platform" << std::endl;
    return 0;
}

void analogWrite(unsigned int pin, int value) {
    std::cout << "analogWrite is not implemented for this platform" << std::endl;
}

// Don't forget to clean up
void cleanup() {
    if (line_request) {
        gpiod_line_request_release(line_request);
        line_request = nullptr;
    }
    if (line_config) {
        gpiod_line_config_free(line_config);
        line_config = nullptr;
    }
    if (chip) {
        gpiod_chip_close(chip);
        chip = nullptr;
    }
}