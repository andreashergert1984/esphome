#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/select/select.h"
#include "select/vaillant_x6_select.h"
#include "esphome/core/automation.h"

namespace esphome {
namespace vaillant_x6 {

struct Vaillant_X6_Command {
  uint8_t command;
  uint8_t data_type;  // out of DataType
  uint8_t response_length;
  uint8_t data_length;
  bool has_status;
  sensor::Sensor *sens;
  binary_sensor::BinarySensor *binary_sens;
  select::Select *select;
  std::string name;
};
struct Vaillant_X6_ScanResult {
  uint8_t command;
  std::vector<uint8_t> result;
};

class Vaillant_x6 : public uart::UARTDevice, public PollingComponent {
 public:
  void addSensor(sensor::Sensor *sens, uint8_t command, uint8_t data_type, uint8_t response_length, uint8_t data_length,
                 bool has_status, std::string name);
  void addSelect(vaillant_x6::VaillantX6Select *selec, uint8_t command, uint8_t data_type, uint8_t response_length,
                 uint8_t data_length, bool has_status, std::string name);
  void addBinarySensor(binary_sensor::BinarySensor *sens, uint8_t command, uint8_t data_type, uint8_t response_length,
                       uint8_t data_length, bool has_status, std::string name);
  void setScan(bool scan) {
    this->scan_ = scan;
    this->scanmode_ = scan;
  };
  //  void switch_command(std::string command);
  void setup() override;
  void loop() override;
  void dump_config() override;
  void update() override;

 protected:
  // static const size_t VAILLANT_X6_READ_BUFFER_LENGTH = 110;  // maximum supported answer length
  static const size_t COMMAND_QUEUE_LENGTH = 10;
  static const size_t COMMAND_TIMEOUT = 2000;
  uint32_t last_poll_ = 0;
  // //  void add_polling_command_(const char *command, ENUMPollingCommand polling_command);
  void empty_uart_buffer_();
  uint8_t calcchecksum(std::vector<uint8_t> command);

  uint8_t check_incoming_checksum_();
  uint8_t check_incoming_length_();
  uint8_t send_next_command_();
  void send_next_poll_();
  void send_next_scan_();
  // void queue_command_(const char *command, uint8_t length);
  std::string command_queue_[COMMAND_QUEUE_LENGTH];
  uint8_t command_queue_position_ = 0;
  std::vector<uint8_t> read_buffer_;
  bool scan_{false};
  bool scanmode_{false};
  uint8_t scaned_command_{0x0};

  std::vector<Vaillant_X6_ScanResult> scan_results_;
  // uint8_t read_buffer_[VAILLANT_X6_READ_BUFFER_LENGTH];
  // size_t read_pos_{0};

  uint32_t command_start_millis_ = 0;
  float fvalue_ = 0;
  bool bvalue_ = false;
  uint8_t sensorstate_ = 0;  // OK
  uint8_t state_;
  enum Sensor_State { OK = 0, SHORT = 1, INTERRUPTED = 2 };
  enum State {
    STATE_IDLE = 0,
    STATE_POLL = 1,
    STATE_COMMAND = 2,
    STATE_POLL_COMPLETE = 3,
    STATE_COMMAND_COMPLETE = 4,
    STATE_POLL_CHECKED = 5,
    STATE_POLL_DECODED = 6,
    STATE_SCAN_POLL = 7,
    STATE_SCAN_COMPLETE = 8,
    STATE_SCAN_CHECKED = 9,
  };
  enum DataType {
    BOOL = 0,
    INT = 1,
    FLOAT = 2,
  };
  uint8_t last_polling_command_ = 0;

  std::vector<Vaillant_X6_Command> used_polling_commands;
};

}  // namespace vaillant_x6
}  // namespace esphome
