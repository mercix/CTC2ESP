#include "dp_component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dp_component {

static const char *const TAG = "dp_component";

void DpComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DP Component...");
}

void DpComponent::update() {
  if (available()) {
    read_data();
  }
}

void DpComponent::read_data() {
  const size_t max_bytes = 10;
  char hex_string[(2 * max_bytes) + 1] = {0};
  char *hex_ptr = hex_string;
  bool valid_start_byte = false;
  bool valid_6th_byte = false;

  ESP_LOGD(TAG, "Reading data from UART...");

  while ((hex_ptr - hex_string) < (2 * max_bytes) && available()) {
    uint8_t byte;
    if (read_byte(&byte)) {
      if ((hex_ptr - hex_string) == 0 && byte == 0xFA) {
        valid_start_byte = true;
        ESP_LOGD(TAG, "Start byte detected: 0xFA");
      }

      if (valid_start_byte) {
        hex_ptr += snprintf(hex_ptr, 3, "%02X", byte);
        ESP_LOGD(TAG, "Received byte: %02X", byte);
      }
    }
  }

  *hex_ptr = '\0';
  ESP_LOGD(TAG, "Received UART string: %s", hex_string);

  if (valid_start_byte && strlen(hex_string) == 12) {
    valid_6th_byte = (hex_string[10] == '0' && hex_string[11] == '0');
    if (valid_6th_byte) {
      process_hex_string(hex_string);
    }
  }
}

void DpComponent::process_hex_string(const char *hex_string) {
  std::vector<uint8_t> h_byte;
  std::vector<std::vector<bool>> b_byte;

  for (size_t x = 2; x < 6; x += 2) {
    h_byte.push_back(static_cast<uint8_t>(std::stoi(std::string(hex_string + x, 2), nullptr, 16)));
    b_byte.push_back(get_bits(h_byte.back()));
  }

  if (compressor) compressor->publish_state(b_byte[0][0]);
  if (fan_low) fan_low->publish_state(b_byte[0][1]);
  if (fan_high) fan_high->publish_state(b_byte[0][3]);
  if (circulation_pump_hp) circulation_pump_hp->publish_state(b_byte[1][1]);
  if (supplementary_heating) supplementary_heating->publish_state(!b_byte[1][5]);
  if (alarm_led) alarm_led->publish_state(b_byte[1][0]);
}

std::vector<bool> DpComponent::get_bits(uint8_t value) {
  std::vector<bool> bits(8);
  for (int i = 0; i < 8; ++i) {
    bits[i] = value & (1 << (7 - i));
  }
  return bits;
}

}  // namespace dp_component
}  // namespace esphome
