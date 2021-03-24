#include "total_energy.h"
#include "esphome/core/log.h"

namespace esphome {
namespace total_energy {

static const char *TAG = "total_energy";

void TotalEnergy::setup() {
  this->pref_ = global_preferences.make_preference<float>(this->get_object_id_hash());

  float recovered;
  if (this->pref_.load(&recovered)) {
    this->publish_state_and_save(recovered);
  } else {
    this->publish_state_and_save(0);
  }
  this->last_update_ = millis();

  this->parent_->add_on_state_callback([this](float state) { this->process_new_state_(state); });
}
void TotalEnergy::dump_config() { LOG_SENSOR("", "Total Energy", this); }
void TotalEnergy::reset() {
  this->total_energy_ = 0;
  this->publish_state_and_save(0);
}
void TotalEnergy::loop() {
  auto t = this->time_->now();
  if (!t.is_valid())
    return;
}
void TotalEnergy::publish_state_and_save(float state) {
  this->pref_.save(&state);
  this->total_energy_ = state;
  this->publish_state(state);
}
void TotalEnergy::process_new_state_(float state) {
  if (isnan(state))
    return;
  const uint32_t now = millis();
  float delta_hours = (now - this->last_update_) / 1000.0f / 60.0f / 60.0f;
  this->last_update_ = now;
  this->publish_state_and_save(this->total_energy_ + state * delta_hours);
}

}  // namespace total_energy
}  // namespace esphome
