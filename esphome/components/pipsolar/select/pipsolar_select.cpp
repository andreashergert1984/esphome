#include "pipsolar_select.h"
#include "esphome/core/log.h"

namespace esphome {
namespace pipsolar {

static const char *const TAG = "pipsolar.select";

void PipsolarSelect::dump_config() { LOG_SELECT(TAG, "Pipsolar Controller Select", this); }

void PipsolarSelect::control(const std::string &value) {
  ESP_LOGD(TAG, "got option: %s",value.c_str());

  // auto options = this->traits.get_options();
  // auto opt_it = std::find(options.cbegin(), options.cend(), value);
  // size_t idx = std::distance(options.cbegin(), opt_it);
  // std::string mapval = this->mapping_[idx];
  // ESP_LOGD(TAG, "Found value %s for option '%s'", *mapval.c_str(), value.c_str());

  if (this->optimistic_)
    this->publish_state(value);
}

}  // namespace Pipsolar_controller
}  // namespace esphome
