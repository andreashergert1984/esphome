#pragma once

#include "../mega2560fanservocontroller.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace mega2560fanservocontroller {

class Mega2560FanServoController;

class Mega2560FanServoControllerSensor : public sensor::Sensor {
 public:
  Mega2560FanServoControllerSensor() {}
  void set_parent(Mega2560FanServoController *parent) { this->parent_ = parent; }
  void set_channel(uint8_t channel) { this->channel_ = channel; }
  void set_port(uint8_t port) { this->port_ = port; }
  void set_sample_time(uint8_t sample_time) { this->sample_time_ = sample_time; }

 protected:
  uint8_t channel_{0}, port_{0}, sample_time_{30};
  // void write_state(float state) override;
  Mega2560FanServoController *parent_;
};

}  // namespace mega2560fanservocontroller
}  // namespace esphome
