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
  // void set_value(float value) { this->write_state(value); };

 protected:
  uint8_t channel_{0}, port_{0}, sample_time_{30};
  // void write_state(float state) override;
  Mega2560FanServoController *parent_;
};

// template<typename... Ts> class SetOutputAction : public Action<Ts...> {
//  public:
//   SetOutputAction(Mega2560FanServoControllerOutput *output) : output_(output) {}

//   TEMPLATABLE_VALUE(float, level)

//   void play(Ts... x) override { this->output_->set_value(this->level_.value(x...)); }

//  protected:
//   Mega2560FanServoControllerOutput *output_;
// };

}  // namespace mega2560fanservocontroller
}  // namespace esphome
