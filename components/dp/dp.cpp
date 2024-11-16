#include "dp.h"

DP::DP(UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {
  compressor = new binary_sensor::BinarySensor();
  fan_low = new binary_sensor::BinarySensor();
  fan_high = new binary_sensor::BinarySensor();
  circulation_pump_hp = new binary_sensor::BinarySensor();
  supplementary_heating = new binary_sensor::BinarySensor();
  alarm_led = new binary_sensor::BinarySensor();
}

void DP::setup() {
  // Additional setup if needed
}

void DP::update() {
  if (available()) {
    readData();
  }
  publishSensorStates();
}

void DP::readData() {
  const size_t maxBytes = 10;
  char hexString[(2 * maxBytes) + 1];
  char *hexPtr = hexString;
  bool validStartByte = false;
  bool valid6thByte = false;

  while ((hexPtr - hexString) < (2 * maxBytes) && available()) {
    uint8_t byte;
    if (read_byte(&byte)) {
      if ((hexPtr - hexString) == 0 && byte == 0xFA) {
        validStartByte = true;
      }

      if (validStartByte) {
        hexPtr += snprintf(hexPtr, 3, "%02X", byte);
        ESP_LOGD("CustomSensor", "Received byte: %02X", static_cast<unsigned>(byte));

        if ((hexPtr - hexString) == 12 && byte == 0x00) {
          valid6thByte = true;
        }
      }
    }
  }

  *hexPtr = '\0';

  if (validStartByte && valid6thByte && (hexPtr - hexString) > 0) {
    ESP_LOGD("CustomSensor", "Received data: %s", hexString);
    processHexString(hexString);
  } else {
    ESP_LOGD("CustomSensor", "Invalid data received.");
  }
}

void DP::processHexString(char *hexString) {
  std::vector<uint8_t> h_byte;
  std::vector<std::vector<bool>> b_byte;

  for (size_t x = 2; x < 6; x += 2) {
    h_byte.push_back(static_cast<uint8_t>(std::stoi(std::string(hexString + x, 2), nullptr, 16)));
    std::vector<bool> bits = getBits(h_byte.back());

    ESP_LOGD("CustomSensor", "Binary representation for Byte %zu:", x / 2 - 1);
    for (size_t i = 0; i < bits.size(); ++i) {
      ESP_LOGD("CustomSensor", "   Bit %zu: %d", i, bits[i] ? 1 : 0);
    }

    b_byte.push_back(bits);
  }

  compressor->publish_state(b_byte[0][0]);
  fan_low->publish_state(b_byte[0][1]);
  fan_high->publish_state(b_byte[0][3]);
  circulation_pump_hp->publish_state(b_byte[1][1]);
  supplementary_heating->publish_state(b_byte[1][5]);
  alarm_led->publish_state(b_byte[1][0]);
}

std::vector<bool> DP::getBits(uint8_t value) {
  std::vector<bool> bits(8);
  for (int i = 7; i >= 0; --i) {
    bits[i] = (value & (1 << (7 - i))) != 0;
  }
  return bits;
}

void DP::publishSensorStates() {
  // Implement sensor state publishing logic if needed
}
