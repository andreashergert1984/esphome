#include "mega2560extender.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mega2560extender {

static const char *const TAG = "mega2560extender";

void Mega2560Extender::setup() {
  this->state_ = STATE_IDLE;
  this->command_start_millis_ = 0;
}


void Mega2560Extender::loop() {
}

void Mega2560Extender::dump_config() {
  ESP_LOGCONFIG(TAG, "Mega2560Extender:");
}
void Mega2560Extender::update() {}


}  // namespace Mega2560Extender
}  // namespace esphome
