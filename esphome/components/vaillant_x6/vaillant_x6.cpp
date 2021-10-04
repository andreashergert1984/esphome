#include "vaillant_x6.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vaillant_x6 {

static const char *TAG = "vaillant_x6";

void Vaillant_x6::setup() {
  this->state_ = STATE_IDLE;
  this->command_start_millis_ = 0;
}

uint8_t Vaillant_x6::calcchecksum(std::vector<uint8_t> command) {
    uint8_t checksum = 0;
        for (auto b: command) {
            if (checksum & 0x80) {
                checksum = (checksum << 1 | 1) & 0xFF;
                checksum = checksum ^ 0x18;

            } else {
                checksum = checksum << 1;   
            }
            checksum = checksum ^ b;
        }
    return checksum;
}


void Vaillant_x6::empty_uart_buffer_() {
  uint8_t byte;
  while (this->available()) {
    this->read_byte(&byte);
  }
}


void Vaillant_x6::addSensor(sensor::Sensor *sens, uint8_t command, uint8_t data_type, uint8_t response_length,
                            uint8_t data_length, bool has_status, std::string name) {
  Vaillant_X6_Command new_command;
  new_command.command = command;
  new_command.data_type = data_type;
  new_command.has_status = has_status;
  new_command.sens = sens;
  new_command.binary_sens = NULL;
  new_command.response_length = response_length;
  new_command.data_length = data_length;
  new_command.name = name;
  this->used_polling_commands.push_back(new_command);
}
void Vaillant_x6::addBinarySensor(binary_sensor::BinarySensor *sens, uint8_t command, uint8_t data_type, uint8_t response_length,
                            uint8_t data_length, bool has_status, std::string name) {
  Vaillant_X6_Command new_command;
  new_command.command = command;
  new_command.data_type = data_type;
  new_command.has_status = has_status;
  new_command.binary_sens = sens;
  new_command.sens = NULL;
  new_command.response_length = response_length;
  new_command.data_length = data_length;
  new_command.name = name;
  this->used_polling_commands.push_back(new_command);
}

void Vaillant_x6::loop() {
  if (this->state_ == STATE_IDLE) {
    this->empty_uart_buffer_();
    switch (this->send_next_command_()) {
      case 0:
        // no command send (empty queue) time to poll
        if (millis() - this->last_poll_ > this->update_interval_) {
          this->send_next_poll_();
          this->last_poll_ = millis();
        }
        return;
        break;
      case 1:
        // command send
        return;
        break;
    }
  }

  // if (this->state_ == STATE_COMMAND_COMPLETE) {
  //   // if (this->check_incoming_length_(4)) {
  //   //   ESP_LOGD(TAG, "response length for command OK");
  //   //   if (this->check_incoming_crc_()) {
  //   //     // crc ok
  //   //     if (this->read_buffer_[1] == 'A' && this->read_buffer_[2] == 'C' && this->read_buffer_[3] == 'K') {
  //   //       ESP_LOGD(TAG, "command successful");
  //   //     } else {
  //   //       ESP_LOGD(TAG, "command not successful");
  //   //     }
  //   //     this->command_queue_[this->command_queue_position_] = std::string("");
  //   //     this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
  //   //     this->state_ = STATE_IDLE;

  //   //   } else {
  //   //     // crc failed
  //   //     this->command_queue_[this->command_queue_position_] = std::string("");
  //   //     this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
  //   //     this->state_ = STATE_IDLE;
  //   //   }
  //   // } else {
  //   //   ESP_LOGD(TAG, "response length for command %s not OK: with length %zu",
  //   //            this->command_queue_[this->command_queue_position_].c_str(), this->read_pos_);
  //   //   this->command_queue_[this->command_queue_position_] = std::string("");
  //   //   this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
  //   //   this->state_ = STATE_IDLE;
  //   // }
  // }

  if (this->state_ == STATE_POLL_DECODED) {
    Vaillant_X6_Command cur = this->used_polling_commands.at(this->last_polling_command_);
    if (cur.has_status) {
      switch (sensorstate_) {
        case OK:
          if (cur.sens) { cur.sens->publish_state(fvalue_); }
          if (cur.binary_sens) { cur.binary_sens->publish_state(bvalue_); }

          break;
        case SHORT:
          // TODO
          break;
        case INTERRUPTED:
          // TODO
          break;
      }

    } else {
          if (cur.sens) { cur.sens->publish_state(fvalue_); }
          if (cur.binary_sens) { cur.binary_sens->publish_state(bvalue_); }
    }
    this->state_ = STATE_IDLE;

  }

  if (this->state_ == STATE_POLL_CHECKED) {
    //decode the response
    Vaillant_X6_Command cur = this->used_polling_commands.at(this->last_polling_command_);
    size_t used_bytes = 2;  // length and unknown second byte ;)
    switch (cur.data_type) {
      case FLOAT:
        if (cur.data_length == 1)
        {
          if (read_buffer.size() >= 2) {
            used_bytes = 3;
            int16_t shift_value = read_buffer.at(2);
            fvalue_ = shift_value;
          }
        }
        if (cur.data_length == 2)
        {
          if (read_buffer.size() >= 3) {
            used_bytes = 4;
            int16_t shift_value = read_buffer.at(2) << 8 | read_buffer.at(3);
            fvalue_ = shift_value / 16.0f;
          }
        }
        break;
      case INT:
        break;
      case BOOL:
          if (read_buffer.size()>2) {
            used_bytes = 3;
            switch (read_buffer.at(2)) {
              case 0x00:
              case 0xF0:
                bvalue_ = false;
                break;
              case 0x0F:
              case 0x01:
                bvalue_ = true;
                break;
            }
          }
        break;
    }
    if (cur.has_status) {
      if (read_buffer.size() >= used_bytes) {}
      switch (read_buffer.at(used_bytes)) {
        case 0x00:
          // OK
          sensorstate_ = OK;
          break;
        case 0x55:
          // short
          sensorstate_ = SHORT;
          break;
        case 0xAA:
          // interrupted
          sensorstate_ = INTERRUPTED;
          break;
      }
    }

    this->state_ = STATE_POLL_DECODED;
    return;
  }

  if (this->state_ == STATE_POLL_COMPLETE) {
    if (this->check_incoming_length_() && this->check_incoming_checksum_()) {
      // if (this->read_buffer_[0] == '(' && this->read_buffer_[1] == 'N' && this->read_buffer_[2] == 'A' &&
      //     this->read_buffer_[3] == 'K') {
      //   this->state_ = STATE_IDLE;
      //   return;
      // }
      // crc ok
      this->state_ = STATE_POLL_CHECKED;
      return;
    } else { 
      this->state_ = STATE_IDLE;
    }
  }

  if (this->state_ == STATE_COMMAND || this->state_ == STATE_POLL) {
    while (this->available()) {
      uint8_t byte;
      this->read_byte(&byte);
      read_buffer.push_back(byte);
      ESP_LOGD(TAG, "got byte: 0x%02x need %d have %d",byte,used_polling_commands.at(last_polling_command_).response_length,read_buffer.size());

      // end of answer
      if (read_buffer.size() == used_polling_commands.at(last_polling_command_).response_length) {
        this->empty_uart_buffer_();
        if (this->state_ == STATE_POLL) {
          this->state_ = STATE_POLL_COMPLETE;
        }
        if (this->state_ == STATE_COMMAND) {
          this->state_ = STATE_COMMAND_COMPLETE;
        }
      }
    }  // available
  }
  // if (this->state_ == STATE_COMMAND) {
  //   if (millis() - this->command_start_millis_ > esphome::vaillant_x6::Vaillant_x6::COMMAND_TIMEOUT) {
  //     // command timeout
  //     const char *command = this->command_queue_[this->command_queue_position_].c_str();
  //     this->command_start_millis_ = millis();
  //     ESP_LOGD(TAG, "timeout command from queue: %s", command);
  //     this->command_queue_[this->command_queue_position_] = std::string("");
  //     this->command_queue_position_ = (command_queue_position_ + 1) % COMMAND_QUEUE_LENGTH;
  //     this->state_ = STATE_IDLE;
  //     return;
  //   } else {
  //   }
  // }
  if (this->state_ == STATE_POLL) {
    if (millis() - this->command_start_millis_ > esphome::vaillant_x6::Vaillant_x6::COMMAND_TIMEOUT) {
      // command timeout
      ESP_LOGD(TAG, "timeout command to poll: %s",
               this->used_polling_commands.at(this->last_polling_command_).name.c_str());
      this->state_ = STATE_IDLE;
    } else {
    }
  }
}

uint8_t Vaillant_x6::check_incoming_length_() {
  ESP_LOGD(TAG, "checking length on incoming message");
  if (read_buffer.at(0) == read_buffer.size()) {
    ESP_LOGD(TAG, "length OK on incoming message");
    return 1;
  }
  ESP_LOGD(TAG, "length FAILED on incoming message");

  return 0;
}

uint8_t Vaillant_x6::check_incoming_checksum_() {
  // TODO: Checksum
  ESP_LOGD(TAG, "checking checksum on incoming message");
  uint8_t checksum = this->read_buffer.back();
  this->read_buffer.pop_back();
  uint8_t should_checksum = this->calcchecksum(read_buffer);
  this->read_buffer.push_back(checksum);
  if (should_checksum == checksum) {
    ESP_LOGD(TAG, "CRC OK");
    return 1;
  }
  ESP_LOGD(TAG, "CRC NOK ");
  return 0;
}

// send next command used
uint8_t Vaillant_x6::send_next_command_() {
  if (this->command_queue_[this->command_queue_position_].length() != 0) {
    // commands not yet implemented as doc is missing
    return 1;
  }
  return 0;
}

void Vaillant_x6::send_next_poll_() {
  this->last_polling_command_ = (this->last_polling_command_ + 1) % this->used_polling_commands.size();
  this->state_ = STATE_POLL;
  this->command_start_millis_ = millis();
  this->empty_uart_buffer_();
  std::vector<uint8_t>().swap(read_buffer);

  // this->read_pos_ = 0;
  Vaillant_X6_Command cur = this->used_polling_commands.at(this->last_polling_command_);
  std::vector<uint8_t> commandpackage;
  commandpackage.push_back(0x07); // first byte is always 0x07
  commandpackage.push_back(0x00); // second byte is always 0x00
  commandpackage.push_back(0x00); // third byte is always 0x00
  commandpackage.push_back(0x00); // fourth byte is always 0x00
  commandpackage.push_back(cur.command);  //the actual command
  commandpackage.push_back(cur.response_length);  //the expected response length
  commandpackage.push_back(this->calcchecksum(commandpackage)); //ending with the checksum
  for (auto cbyte: commandpackage) {
    this->write(cbyte);
  }
  ESP_LOGD(TAG, "Sending polling command : %s with length %d", cur.name.c_str(), cur.data_length);
}

// void Vaillant_x6::queue_command_(const char *command, uint8_t length) {
//   // uint8_t next_position = command_queue_position_;
//   // for (uint8_t i = 0; i < COMMAND_QUEUE_LENGTH; i++) {
//   //   uint8_t testposition = (next_position + i) % COMMAND_QUEUE_LENGTH;
//   //   if (command_queue_[testposition].length() == 0) {
//   //     command_queue_[testposition] = command;
//   //     ESP_LOGD(TAG, "Command queued successfully: %s with length %lu at position %d", command,
//   //              command_queue_[testposition].length(), testposition);
//   //     return;
//   //   }
//   // }
//   // ESP_LOGD(TAG, "Command queue full dropping command: %s", command);
// }

// void Vaillant_x6::switch_command(std::string command) {
//   ESP_LOGD(TAG, "got command: %s", command.c_str());
//   queue_command_(command.c_str(), command.length());
// }

void Vaillant_x6::dump_config() {
  ESP_LOGCONFIG(TAG, "Vaillant_x6:");
  ESP_LOGCONFIG(TAG, "used commands:");
  for (const auto used_polling_command : this->used_polling_commands) {
    ESP_LOGCONFIG(TAG, "%s: 0x%x", used_polling_command.name.c_str(), used_polling_command.command);
  }
}
void Vaillant_x6::update() {}

// void Vaillant_x6::add_polling_command_(const char* command, ENUMPollingCommand polling_command) {
//   for (auto& used_polling_command : this->used_polling_commands_) {
//     if (used_polling_command.length == strlen(command)) {
//       uint8_t len = strlen(command);
//       if (memcmp(used_polling_command.command, command, len) == 0) {
//         return;
//       }
//     }
//     if (used_polling_command.length == 0) {
//       size_t length = strlen(command) + 1;
//       const char* beg = command;
//       const char* end = command + length;
//       used_polling_command.command = new uint8_t[length];
//       size_t i = 0;
//       for (; beg != end; ++beg, ++i) {
//         used_polling_command.command[i] = (uint8_t) (*beg);
//       }
//       used_polling_command.errors = 0;
//       used_polling_command.identifier = polling_command;
//       used_polling_command.length = length - 1;
//       return;
//     }
//   }
// }

}  // namespace vaillant_x6
}  // namespace esphome
