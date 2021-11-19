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
  ESP_LOGD(TAG, "Setting up Mega2560FanServoControllerComponent...");
  //
  // PWM Output :
  // Timer1 Address : 0x01
  // Timer3 Address : 0x02
  // Timer4 Address : 0x03
  // Timer5 Address : 0x04

  // Timer Bytes :
  // 0 : Enabled
  // 1 : 50Hz Output Enabled(prio)
  // 2 : 25kHz Output Enabled
  // 3 : Port A Enabled
  // 4 : Port B Enabled
  // 5 : Port C Enabled
  // 6 : not used
  // 7 : not used

  ESP_LOGD(TAG, "Setting up all timers for Output");
  for (uint8_t channel = 0; channel < 4; channel++) {
    ESP_LOGD(TAG, "Setting up all timers for Output %d", channel);
    uint8_t channelconfig = 0;  // all off
    if (this->pwm_frequency_[channel] == 50) {
      channelconfig |= (1 << 1);
    }  // Set 50Hz Mode
    if (this->pwm_frequency_[channel] == 25000) {
      channelconfig |= (1 << 2);
    }  // Set 25000Hz Mode
    // enable needed outputs
    channelconfig |= (this->pwm_port_enabled_[channel * 3] << 3) & (this->pwm_port_enabled_[channel * 3 + 1] << 4) &
                     (this->pwm_port_enabled_[channel * 3 + 2] << 5);
    uint8_t req_register = 0x00 + channel;
    if (!this->write_byte(req_register, channelconfig)) {
      //      this->mark_failed();
    }
  }

  this->loop();
}

void Mega2560FanServoController::dump_config() {
  ESP_LOGCONFIG(TAG, "Mega2560FanServoController dingens:");

  for (auto channel = 0; channel < 4; channel++) {
    ESP_LOGCONFIG(TAG, "PWM Channel %d Setup to %dHz", channel, this->pwm_frequency_[channel]);
  }
  // ESP_LOGCONFIG(TAG, "  Mode: 0x%02X", this->mode_);
  // ESP_LOGCONFIG(TAG, "  Frequency: %.0f Hz", this->frequency_);
  if (this->is_failed()) {
    ESP_LOGCONFIG(TAG, "Setting up Mega2560FanServoController failed!");
  }
}

void Mega2560FanServoController::write_pwm(uint8_t channel, uint8_t port, float speed) {
  this->pwm_amounts_[channel * 3 + port] = (uint8_t) (speed * 255.0f);
  if (!this->write_byte(0x10 + channel * 3 + port, this->pwm_amounts_[channel * 3 + port])) {
    //    this->mark_failed();
  }
}

void Mega2560FanServoController::loop() {}

void Mega2560FanServoController::update() {}

}  // namespace mega2560fanservocontroller
}  // namespace esphome
