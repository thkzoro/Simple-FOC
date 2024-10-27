#include <string>
#include <thread>
#include <map>
#include <memory>
#include "PWM.h"

constexpr int PWM_PERIOD_NS = 20000; //50KHz
 // first 是 chip，second 是 channel
 ChipChannel PWMManager::getChipChannel(int pin) {
     switch (pin) {
         case 0:
             return {0, 0};
         case 1:
             return {0, 1};
         case 6:
             return {2, 0};
         case 7:
             return {2, 1};
         default:
             return {0, 0};
     }
 }

PWM::PWM(int pin){
    ChipChannel chip_channel = PWMManager::getChipChannel(pin);
    auto chip = chip_channel.first;
    auto channel = chip_channel.second;
    std::string base_path = "/sys/class/pwm/pwmchip" + std::to_string(chip);
    std::ofstream f_export(base_path + "/export");
    f_export << channel;
    f_export.close();

    auto pwm_path = base_path + "/pwm" + std::to_string(channel);
    std::ofstream f_period(pwm_path + "/period");
    f_period << std::to_string(PWM_PERIOD_NS);
    f_period.close();

    m_f_duty_cycle.open(pwm_path + "/duty_cycle");
    m_f_duty_cycle << "0";

    std::ofstream f_enable(pwm_path + "/enable");
    f_enable << "1";
    f_enable.close();
}

void PWM::set_duty_cycle(double duty_cycle_percent) {
    if (duty_cycle_percent < 0.0) duty_cycle_percent = 0.0;
    if (duty_cycle_percent > 100.0) duty_cycle_percent = 100.0;
    auto duty_cycle_ns = static_cast<unsigned long long>(PWM_PERIOD_NS * duty_cycle_percent / 100.0);
    m_f_duty_cycle << duty_cycle_ns;
    m_f_duty_cycle.flush();
}

void analogWrite(unsigned int pin, int value) {
    auto pwm = PWMManager::getInstance().getPWM(pin);
    if (pwm) {
        double duty_cycle_percent = value * 100.0 / 255.0;
        pwm->set_duty_cycle(duty_cycle_percent);
    }
}