#include "mega2560fanservocontroller_output.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace mega2560fanservocontroller {

static const char *const TAG = "Mega2560FanServoController.output";

void Mega2560FanServoControllerOutput::write_state(float state) {
  this->parent_->write_pwm(this->channel_, this->port_, state);
}
}  // namespace mega2560fanservocontroller
}  // namespace esphome
