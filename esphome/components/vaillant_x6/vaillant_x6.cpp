#include "vaillant_x6.h"
#include "esphome/core/log.h"

namespace esphome {
namespace vaillant_x6 {

static const char *TAG = "vaillant_x6";

void Vaillant_x6::setup() {
  this->state_ = STATE_IDLE;
  // this->command_start_millis_ = 0;
}

void Vaillant_x6::empty_uart_buffer_() {
  uint8_t byte;
  while (this->available()) {
    this->read_byte(&byte);
  }
}

void Vaillant_x6::addSensor(sensor::Sensor *sens, uint8_t command, std::string data_type, uint8_t response_length,
                            uint8_t data_length, bool has_status, std::string name) {
  Vaillant_X6_Command new_command;
  new_command.command = command;
  new_command.data_type = data_type;
  new_command.has_status = has_status;
  new_command.sens = sens;
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

  // if (this->state_ == STATE_POLL_DECODED) {
  // }

  // if (this->state_ == STATE_POLL_CHECKED) {
  //   return;
  // }

  // if (this->state_ == STATE_POLL_COMPLETE) {
  //   if (this->check_incoming_crc_()) {
  //     if (this->read_buffer_[0] == '(' && this->read_buffer_[1] == 'N' && this->read_buffer_[2] == 'A' &&
  //         this->read_buffer_[3] == 'K') {
  //       this->state_ = STATE_IDLE;
  //       return;
  //     }
  //     // crc ok
  //     this->state_ = STATE_POLL_CHECKED;
  //     return;
  //   } else { 
  //     this->state_ = STATE_IDLE;
  //   }
  // }

  if (this->state_ == STATE_COMMAND || this->state_ == STATE_POLL) {
    while (this->available()) {
      uint8_t byte;
      this->read_byte(&byte);

      if (this->read_pos_ == VAILLANT_X6_READ_BUFFER_LENGTH) {
        this->read_pos_ = 0;
        this->empty_uart_buffer_();
      }
      this->read_buffer_[this->read_pos_] = byte;
      this->read_pos_++;

      // end of answer
      if (byte == 0x0D) {
        this->read_buffer_[this->read_pos_] = 0;
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

// uint8_t Vaillant_x6::check_incoming_length_(uint8_t length) {
//   if (this->read_pos_ - 3 == length) {
//     return 1;
//   }
//   return 0;
// }

// uint8_t Vaillant_x6::check_incoming_crc_() {
//   // uint16_t crc16;
//   // crc16 = calc_crc_(read_buffer_, read_pos_ - 3);
//   // ESP_LOGD(TAG, "checking crc on incoming message");
//   // if (((uint8_t) ((crc16) >> 8)) == read_buffer_[read_pos_ - 3] &&
//   //     ((uint8_t) ((crc16) &0xff)) == read_buffer_[read_pos_ - 2]) {
//   //   ESP_LOGD(TAG, "CRC OK");
//   //   read_buffer_[read_pos_ - 1] = 0;
//   //   read_buffer_[read_pos_ - 2] = 0;
//   //   read_buffer_[read_pos_ - 3] = 0;
//   //   return 1;
//   // }
//   // ESP_LOGD(TAG, "CRC NOK expected: %X %X but got: %X %X", ((uint8_t) ((crc16) >> 8)), ((uint8_t) ((crc16) &0xff)),
//   //          read_buffer_[read_pos_ - 3], read_buffer_[read_pos_ - 2]);
//   return 0;
// }

// send next command used
uint8_t Vaillant_x6::send_next_command_() {
  // uint16_t crc16;
  if (this->command_queue_[this->command_queue_position_].length() != 0) {
    //   const char* command = this->command_queue_[this->command_queue_position_].c_str();
    //   uint8_t byte_command[16];
    //   uint8_t length = this->command_queue_[this->command_queue_position_].length();
    //   for (uint8_t i = 0; i < length; i++) {
    //     byte_command[i] = (uint8_t) this->command_queue_[this->command_queue_position_].at(i);
    //   }
    //   this->state_ = STATE_COMMAND;
    //   this->command_start_millis_ = millis();
    //   this->empty_uart_buffer_();
    //   this->read_pos_ = 0;
    //   crc16 = calc_crc_(byte_command, length);
    //   this->write_str(command);
    //   // checksum
    //   this->write(((uint8_t) ((crc16) >> 8)));   // highbyte
    //   this->write(((uint8_t) ((crc16) &0xff)));  // lowbyte
    //   // end Byte
    //   this->write(0x0D);
    //   ESP_LOGD(TAG, "Sending command from queue: %s with length %d", command, length);
    return 1;
  }
  return 0;
}

void Vaillant_x6::send_next_poll_() {
  // uint16_t crc16;
  this->last_polling_command_ = (this->last_polling_command_ + 1) % this->used_polling_commands.size();
  this->state_ = STATE_POLL;
  this->command_start_millis_ = millis();
  this->empty_uart_buffer_();
  this->read_pos_ = 0;
  Vaillant_X6_Command cur = this->used_polling_commands.at(this->last_polling_command_);
  // crc16 = calc_crc_(this->used_polling_commands_[this->last_polling_command_].command,
  //                   this->used_polling_commands_[this->last_polling_command_].length);
  // this->write_array(this->used_polling_commands_[this->last_polling_command_].command,
  //                   this->used_polling_commands_[this->last_polling_command_].length);
  // // checksum
  // this->write(((uint8_t) ((crc16) >> 8)));   // highbyte
  // this->write(((uint8_t) ((crc16) &0xff)));  // lowbyte
  // // end Byte
  // this->write(0x0D);
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
