#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/automation.h"


namespace esphome {
namespace vaillant_x6 {

struct Vaillant_X6_Command {
  uint8_t command;
  std::string data_type;
  uint8_t response_length;
  uint8_t data_length;
  bool has_status;
  sensor::Sensor* sens;
};

class Vaillant_x6 : public uart::UARTDevice, public PollingComponent {
 public:
  void set_scan(bool scan) { this->scan_ = scan; }
  //  void set_grid_voltage_sensor(sensor::Sensor *grid_voltage_sensor) {
  //    grid_voltage_ = grid_voltage_sensor;
  //    this->add_polling_command_("QPIGS", POLLING_QPIGS);
  //  }
  void addSensor(sensor::Sensor *sens, uint8_t command, std::string data_type, uint8_t response_length, uint8_t data_length, bool has_status);
  void switch_command(std::string command);
  void setup() override;
  void loop() override;
  void dump_config() override;
  void update() override;

 protected:
  static const size_t VAILLANT_X6_READ_BUFFER_LENGTH = 110;  // maximum supported answer length
  static const size_t COMMAND_QUEUE_LENGTH = 10;
  static const size_t COMMAND_TIMEOUT = 5000;
  bool scan_ = false;
  uint32_t last_poll_ = 0;
  //  void add_polling_command_(const char *command, ENUMPollingCommand polling_command);
  void empty_uart_buffer_();
  uint8_t check_incoming_crc_();
  uint8_t check_incoming_length_(uint8_t length);
  uint8_t send_next_command_();
  void send_next_poll_();
  void queue_command_(const char *command, uint8_t length);
  std::string command_queue_[COMMAND_QUEUE_LENGTH];
  uint8_t command_queue_position_ = 0;
  uint8_t read_buffer_[VAILLANT_X6_READ_BUFFER_LENGTH];
  size_t read_pos_{0};

  uint32_t command_start_millis_ = 0;
  uint8_t state_;
  enum State {
    STATE_IDLE = 0,
    STATE_POLL = 1,
    STATE_COMMAND = 2,
    STATE_POLL_COMPLETE = 3,
    STATE_COMMAND_COMPLETE = 4,
    STATE_POLL_CHECKED = 5,
    STATE_POLL_DECODED = 6,
  };

  uint8_t last_polling_command_ = 0;
  std::vector<Vaillant_X6_Command*> used_polling_commands;

//  PollingCommand used_polling_commands_[15];
};

}  // namespace vaillant_x6
}  // namespace esphome
