#include "dp.h"

void DP::update() {
  if (available()) {
    read_data();
  }
}

void DP::read_data() {
  const size_t maxBytes = 10;
  char hexString[(2 * maxBytes) + 1];
  char *hexPtr = hexString;

  while ((hexPtr - hexString) < (2 * maxBytes) && available()) {
    uint8_t byte;
    if (read_byte(&byte)) {
      hexPtr += snprintf(hexPtr, 3, "%02X", byte);
    }
  }

  *hexPtr = '\0';
  process_data(hexString);
}

void DP::process_data(const char *hex_data) {
  if (strlen(hex_data) < 12) return;

  uint8_t byte1 = strtol(std::string(hex_data + 2, 2).c_str(), nullptr, 16);
  uint8_t byte2 = strtol(std::string(hex_data + 4, 2).c_str(), nullptr, 16);

  if (compressor_) compressor_->publish_state(byte1 & 0x01);
  if (fan_low_) fan_low_->publish_state(byte1 & 0x02);
  if (fan_high_) fan_high_->publish_state(byte1 & 0x08);
  if (alarm_led_) alarm_led_->publish_state(byte2 & 0x01);
  if (circulation_pump_hp_) circulation_pump_hp_->publish_state(byte2 & 0x02);
  if (supplementary_heating_) supplementary_heating_->publish_state(byte2 & 0x20);
}

