#pragma once

#include "esphome/core/component.h"
#include "esphome/components/output/float_output.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace mega2560fanservocontroller {

class Mega2560FanServoController;

class Mega2560FanServoController : public PollingComponent, public i2c::I2CDevice {
 public:
  // void register_channel(PCA9685Channel *channel);
  void set_pwm_channel(uint8_t channel, uint16_t speed) { this->pwm_frequency_[channel] = speed; }
  void write_pwm(uint8_t channel, uint8_t port, float speed);
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void loop() override;
  void update() override;

 protected:
  uint16_t pwm_frequency_[4] = {0};
  uint8_t pwm_amounts_[12] = {
      0,
  };
};

}  // namespace mega2560fanservocontroller
}  // namespace esphome
