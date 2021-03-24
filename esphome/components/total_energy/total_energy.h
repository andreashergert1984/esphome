#pragma once

#include "esphome/core/component.h"
#include "esphome/core/preferences.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/time/real_time_clock.h"

namespace esphome {
namespace total_energy {

class TotalEnergy : public sensor::Sensor, public Component {
 public:
  void set_time(time::RealTimeClock *time) { time_ = time; }
  void set_parent(Sensor *parent) { parent_ = parent; }
  void setup() override;
  void dump_config() override;
  void reset();
  float get_setup_priority() const override { return setup_priority::DATA; }
  std::string unit_of_measurement() override { return this->parent_->get_unit_of_measurement() + "h"; }
  std::string icon() override { return this->parent_->get_icon(); }
  int8_t accuracy_decimals() override { return this->parent_->get_accuracy_decimals() + 2; }
  void loop() override;

  void publish_state_and_save(float state);

 protected:
  void process_new_state_(float state);

  ESPPreferenceObject pref_;
  time::RealTimeClock *time_;
  Sensor *parent_;
  uint16_t last_day_of_year_{};
  uint32_t last_update_{0};
  float total_energy_{0.0f};
};

template<typename... Ts> class TotalEnergyResetAction : public Action<Ts...> {
 public:
  TotalEnergyResetAction(total_energy::TotalEnergy *totalenergy) : totalenergy_(totalenergy) {}

  void play(Ts... x) override { this->totalenergy_->reset(); }

 protected:
  TotalEnergy *totalenergy_;
};


}  // namespace total_energy
}  // namespace esphome
