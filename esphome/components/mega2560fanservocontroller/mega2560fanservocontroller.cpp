#include "mega2560fanservocontroller.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace mega2560fanservocontroller {

static const char *const TAG = "mega2560fanservocontroller";

static const uint8_t PCA9685_REGISTER_SOFTWARE_RESET = 0x06;
static const uint8_t PCA9685_REGISTER_MODE1 = 0x00;
static const uint8_t PCA9685_REGISTER_MODE2 = 0x01;
static const uint8_t PCA9685_REGISTER_LED0 = 0x06;
static const uint8_t PCA9685_REGISTER_PRE_SCALE = 0xFE;

void Mega2560FanServoController::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Mega2560FanServoControllerComponent...");

  // ESP_LOGV(TAG, "  Resetting devices...");
  // if (!this->write_bytes(PCA9685_REGISTER_SOFTWARE_RESET, nullptr, 0)) {
  //   this->mark_failed();
  //   return;
  // }

  // if (!this->write_byte(PCA9685_REGISTER_MODE1, PCA9685_MODE1_RESTART | PCA9685_MODE1_AUTOINC)) {
  //   this->mark_failed();
  //   return;
  // }
  // if (!this->write_byte(PCA9685_REGISTER_MODE2, this->mode_)) {
  //   this->mark_failed();
  //   return;
  // }

  this->loop();
}

void Mega2560FanServoController::dump_config() {
  ESP_LOGCONFIG(TAG, "Mega2560FanServoController:");

  for (auto channel = 0; channel < 4; channel++) {
    ESP_LOGCONFIG(TAG, "PWM Channel %d Setup to %dHz", channel, this->pwm_frequency_[channel]);
  }
  // ESP_LOGCONFIG(TAG, "  Mode: 0x%02X", this->mode_);
  // ESP_LOGCONFIG(TAG, "  Frequency: %.0f Hz", this->frequency_);
  if (this->is_failed()) {
    ESP_LOGE(TAG, "Setting up Mega2560FanServoController failed!");
  }
}

void Mega2560FanServoController::write_pwm(uint8_t channel, uint8_t port, float speed) {
  this->pwm_amounts_[channel * 3 + port] = (uint8_t) (speed * 255.0f);
  if (!this->write_byte(0x10 + channel * 3 + port, this->pwm_amounts_[channel * 3 + port])) {
    this->mark_failed();
  }
}

void Mega2560FanServoController::loop() {}

void Mega2560FanServoController::update() {}

}  // namespace mega2560fanservocontroller
}  // namespace esphome
