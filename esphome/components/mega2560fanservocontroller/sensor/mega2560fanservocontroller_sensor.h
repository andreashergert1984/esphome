#pragma once

#include "../mega2560fanservocontroller.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace mega2560fanservocontroller {

class Mega2560FanServoController;

class Mega2560FanServoControllerSensor : public sensor::Sensor, public PollingComponent {
 public:
  Mega2560FanServoControllerSensor() {}
  void set_parent(Mega2560FanServoController *parent) { this->parent_ = parent; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_port(uint8_t port) { this->port_ = port; }
  void set_update_interval(uint8_t update_interval) { this->update_interval_ = update_interval; }
  void update();

 protected:
  uint8_t channel_{0}, port_{0}, update_interval_{30};
  // void write_state(float state) override;
  Mega2560FanServoController *parent_;
  uint32_t last_update_millis_ = 0;
};

}  // namespace mega2560fanservocontroller
}  // namespace esphome
