#ifndef SIMPLEFOC_LINUX_PWM_H
#define SIMPLEFOC_LINUX_PWM_H

#include <fstream>

using ChipChannel = std::pair<int, int>;
class PWM;
class PWMManager {
public:
    static PWMManager& getInstance() {
        static PWMManager instance;
        return instance;
    }

    static ChipChannel getChipChannel(int pin);

    void addPWM(int pin, const std::shared_ptr<PWM> &pwm) {
        m_pwms[pin] = pwm;
    }
    std::shared_ptr<PWM> getPWM(int pin) {
        if (m_pwms.find(pin) == m_pwms.end()) {
            return nullptr;
        }
        return m_pwms[pin];
    }

private:
    PWMManager() = default;
    PWMManager(const PWMManager&) = delete;
    PWMManager& operator=(const PWMManager&) = delete;

    std::map<int, std::shared_ptr<PWM>> m_pwms;
};

class PWM {
public:
    PWM(int pin);
    ~PWM() {}

    void set_duty_cycle(double duty_cycle_percent);

private:
    std::ofstream m_f_duty_cycle;
};



void analogWrite(unsigned int pin, int value);



#endif //SIMPLEFOC_LINUX_PWM_H
