#include "dp_custom.h"
#include "esphome/core/log.h"

namespace esphome {
namespace dp_custom {

static const char *TAG = "dp_custom";  // Logging tag

// Constructor definition
DP::DP(uart::UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {
  compressor = new binary_sensor::BinarySensor();
  fan_low = new binary_sensor::BinarySensor();
  fan_high = new binary_sensor::BinarySensor();
  circulation_pump_hp = new binary_sensor::BinarySensor();
  supplementary_heating = new binary_sensor::BinarySensor();
  alarm_led = new binary_sensor::BinarySensor();
}

// Setup function
void DP::setup() {
  // Additional setup if needed
}

// Update function
void DP::update() {
  if (this->available()) {
    readData();
  }
  publishSensorStates();
}

// Read data from UART
void DP::readData() {
  const size_t maxBytes = 10;
  char hexString[(2 * maxBytes) + 1];
  char *hexPtr = hexString;
  bool validStartByte = false;
  bool valid6thByte = false;

  ESP_LOGD(TAG, "Reading data from UART...");

  while ((hexPtr - hexString) < (2 * maxBytes) && this->available()) {
    uint8_t byte;
    if (this->read_byte(&byte)) {
      if ((hexPtr - hexString) == 0 && byte == 0xFA) {
        validStartByte = true;
        ESP_LOGD(TAG, "Start byte detected: 0xFA");
      }
      if (validStartByte) {
        hexPtr += snprintf(hexPtr, 3, "%02X", byte);
        ESP_LOGD(TAG, "Received byte: %02X", byte);
      }
    }
  }

  *hexPtr = '\0';

  if (validStartByte && (hexPtr - hexString) > 0) {
    if ((hexPtr - hexString) == 12 && hexString[10] == '0' && hexString[11] == '0') {
      valid6thByte = true;
    }
  }

  if (validStartByte && valid6thByte) {
    ESP_LOGD(TAG, "Received valid UART string: %s", hexString);
    processHexString(hexString);
  } else {
    ESP_LOGD(TAG, "Invalid or incomplete data received.");
  }
}

// Process the received hex string
void DP::processHexString(char *hexString) {
  std::vector<uint8_t> h_byte;
  std::vector<std::vector<bool>> b_byte;

  for (size_t x = 2; x < 6; x += 2) {
    h_byte.push_back(static_cast<uint8_t>(std::stoi(std::string(hexString + x, 2), nullptr, 16)));
    b_byte.push_back(getBits(h_byte.back()));
  }

  ESP_LOGD(TAG, "Publishing sensor states...");

  compressor->publish_state(b_byte[0][0]);
  fan_low->publish_state(b_byte[0][1]);
  fan_high->publish_state(b_byte[0][3]);
  circulation_pump_hp->publish_state(b_byte[1][1]);
  supplementary_heating->publish_state(b_byte[1][5]);
  alarm_led->publish_state(b_byte[1][0]);
}

// Convert the value to bits
std::vector<bool> DP::getBits(uint8_t value) {
  std::vector<bool> bits(8);
  for (int i = 7; i >= 0; --i) {
    bits[i] = (value & (1 << (7 - i))) != 0;
  }
  return bits;
}

// Publish the sensor states
void DP::publishSensorStates() {
  // Placeholder for additional state publishing logic if needed
}

}  // namespace dp_custom
}  // namespace esphome
