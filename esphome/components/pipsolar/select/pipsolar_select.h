#pragma once

#include <utility>

#include "esphome/components/pipsolar/pipsolar.h"
#include "esphome/components/select/select.h"
#include "esphome/core/component.h"

namespace esphome {
namespace pipsolar {
class Pipsolar;

class PipsolarSelect : public Component, public select::Select {
 public:
  PipsolarSelect(
    // uint16_t start_address, uint8_t register_count, uint8_t skip_updates,
    //            bool force_new_range, std::vector<int64_t> mapping
               ) {
    // this->mapping_ = std::move(mapping);
  }

  void set_parent(Pipsolar *const parent) { this->parent_ = parent; }
  void set_optimistic(bool optimistic) { this->optimistic_ = optimistic; }

  void dump_config() override;
  // void parse_and_publish(const std::vector<uint8_t> &data) override;
  void control(const std::string &value) override;

 protected:
  std::vector<std::string> mapping_;
  Pipsolar *parent_;
  bool optimistic_{false};
};

}  // namespace pipsolar
}  // namespace esphome
