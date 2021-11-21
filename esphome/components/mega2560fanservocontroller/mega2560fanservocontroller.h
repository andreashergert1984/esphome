#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "sensor/mega2560fanservocontroller_sensor.h"

namespace esphome {
namespace mega2560fanservocontroller {

class Mega2560FanServoController;
class Mega2560FanServoControllerSensor;

class Mega2560FanServoController : public PollingComponent, public i2c::I2CDevice {
 public:
  // void register_channel(PCA9685Channel *channel);
  void set_pwm_channel(uint8_t channel, uint16_t speed) { this->pwm_frequency_[channel % 12] = speed; }
  void enable_pwm_port(uint8_t channel, uint8_t port) {
    this->pwm_port_enabled_[(channel * 3 + port) % 12] = true;
    //    this->update_config();
  }
  void update_config();
  void write_pwm(uint8_t channel, uint8_t port, float speed);
  void register_rpm_sensor(uint8_t channel, uint8_t port, Mega2560FanServoControllerSensor *sens) {
    this->rpm_sensors[(channel * 3 + port) % 12] = sens;
  };
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE; }
  void loop() override;
  void update() override;

 protected:
  uint16_t pwm_frequency_[4] = {0};
  bool pwm_port_enabled_[12] = {false};
  uint8_t pwm_amounts_[12] = {
      0,
  };
  Mega2560FanServoControllerSensor *rpm_sensors[12] = {NULL};
};

}  // namespace mega2560fanservocontroller
}  // namespace esphome
