#include "esphome.h"
#include <sstream>

class DP : public PollingComponent, public UARTDevice {
public:
  DP(UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {}

  // Declare binary sensors
  binary_sensor::BinarySensor *compressor = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *fan_low = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *fan_high = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *circulation_pump_hp = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *supplementary_heating = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *alarm_led = new binary_sensor::BinarySensor();

  void setup() override {
    // Initial setup, if needed
  }

  void loop() override {
    // No need for loop, ESPHome handles this
  }

  void update() override {
    if (available()) {
      readData();
    }
    publishSensorStates();
  }

  void readData() {
    const size_t maxBytes = 10;
    char hexString[(2 * maxBytes) + 1];  // Adjusted buffer size and null terminator
    char* hexPtr = hexString;  // Pointer to traverse the hexString array
    bool validStartByte = false;
    bool valid6thByte = false;

    ESP_LOGD("CustomSensor", "Reading data from UART...");

    while ((hexPtr - hexString) < (2 * maxBytes) && available()) {
        uint8_t byte;
        if (read_byte(&byte)) {
            if ((hexPtr - hexString) == 0 && byte == 0xFA) {
                validStartByte = true;
                ESP_LOGD("CustomSensor", "Start byte detected: 0xFA");
            }

            if (validStartByte) {
                hexPtr += snprintf(hexPtr, 3, "%02X", byte);  // Use snprintf to format and advance the pointer
                ESP_LOGD("CustomSensor", "Received byte: %02X", static_cast<unsigned>(byte));
            } else {
                ESP_LOGD("CustomSensor", "Invalid byte received before start byte: %02X", static_cast<unsigned>(byte));
            }
        }
    }

    *hexPtr = '\0'; // Null-terminate the string
    ESP_LOGD("CustomSensor", "Received UART string: %s", hexString);

    if (validStartByte && (hexPtr - hexString) > 0) {
        if ((hexPtr - hexString) == 12 && hexString[10] == '0' && hexString[11] == '0') {
            valid6thByte = true;
            ESP_LOGD("CustomSensor", "6th byte detected: 0x00");
        }
    }

    if (validStartByte && valid6thByte && (hexPtr - hexString) > 0) {
        ESP_LOGD("CustomSensor", "Received data: %s", hexString);
        processHexString(hexString);
    } else {
        ESP_LOGD("CustomSensor", "Invalid start byte, invalid 6th byte, or incomplete data received.");
    }
  }

  void processHexString(char* hexString) {
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

    ESP_LOGD("CustomSensor", "Compressor: %c", b_byte[0][0] ? '1' : '0');
    ESP_LOGD("CustomSensor", "Fan low: %c", b_byte[0][1] ? '1' : '0');
    ESP_LOGD("CustomSensor", "Fan high: %c", b_byte[0][3] ? '1' : '0');
    ESP_LOGD("CustomSensor", "Alarm LED: %c", b_byte[1][0] ? '1' : '0');
    ESP_LOGD("CustomSensor", "Circulation pump HP %c", b_byte[1][1] ? '1' : '0');
    ESP_LOGD("CustomSensor", "Supplementary heating(inverted): %c", b_byte[1][5] ? '1' : '0');

    compressor->publish_state(b_byte[0][0] ? 1 : 0);
    fan_low->publish_state(b_byte[0][1] ? 1 : 0);
    fan_high->publish_state(b_byte[0][3] ? 1 : 0);
    circulation_pump_hp->publish_state(b_byte[1][1] ? 1 : 0);
    supplementary_heating->publish_state(b_byte[1][5] ? 1 : 0);
    alarm_led->publish_state(b_byte[1][0] ? 1 : 0);
  }

  std::vector<bool> getBits(uint8_t value) {
    std::vector<bool> bits(8);
    for (int i = 7; i >= 0; --i) {
        bits[i] = (value & (1 << (7 - i))) != 0;
    }
    return bits;
  }

  void publishSensorStates() {
    // Perform actions to publish sensor states
    // You may use ESP_LOGD for debugging
  }
};
