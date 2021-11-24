#include "mega2560fanservocontroller_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace mega2560fanservocontroller {

static const char *const TAG = "Mega2560FanServoController.sensor";

void Mega2560FanServoControllerSensor::update() {
  if (millis() - this->last_update_millis_ > (this->update_interval_ * 1000)) {
    this->last_update_millis_ = millis();
    ESP_LOGD(TAG, "update");

    this->publish_state(this->parent_->get_rpm(this->channel_, this->port_));
  }
}
}  // namespace mega2560fanservocontroller
}  // namespace esphome
