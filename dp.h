#include "esphome.h"
#include <sstream>

class DP : public PollingComponent, public UARTDevice {
public:
  DP(UARTComponent *parent) : PollingComponent(5000), UARTDevice(parent) {}
  
  binary_sensor::BinarySensor *compressor = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *fan_low = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *fan_high = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *circulation_pump_hp = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *supplementary_heating = new binary_sensor::BinarySensor();
  binary_sensor::BinarySensor *alarm_led = new binary_sensor::BinarySensor();
  
  // ... (similar declarations for other sensors)

  void setup() override {
    // Additional setup if needed
  }

  void loop() override {
    // No need to read data in the loop function, all UART reading logic is moved to update
  }

  void update() override {
    // Check if UART data is available
    if (available()) {
        // Read data from the UART stream and process it
        readData();
    }

    // Perform sensor updates and publish states
    publishSensorStates();
  }

  void readData() {
    const size_t maxBytes = 10;
    char hexString[(2 * maxBytes) + 1];  // Adjusted buffer size and null terminator

    char* hexPtr = hexString;  // Pointer to traverse the hexString array
    bool validStartByte = false;
    bool valid6thByte = false;

    while ((hexPtr - hexString) < (2 * maxBytes) && available()) {
        uint8_t byte;
        if (read_byte(&byte)) {
            if ((hexPtr - hexString) == 0 && byte == 0xFA) {
                validStartByte = true;
            }

            if (validStartByte) {
                hexPtr += snprintf(hexPtr, 3, "%02X", byte);  // Use snprintf to format and advance the pointer
                ESP_LOGD("CustomSensor", "Received byte: %02X", static_cast<unsigned>(byte));

                // Check if it's the 6th byte
                if ((hexPtr - hexString) == 12 && byte == 0x00) {
                    valid6thByte = true;
                }
            }
        }
    }

    // Null-terminate the string
    *hexPtr = '\0';

    if (validStartByte && valid6thByte && (hexPtr - hexString) > 0) {
        ESP_LOGD("CustomSensor", "Received data: %s", hexString);
        processHexString(hexString);
    } else {
        ESP_LOGD("CustomSensor", "Invalid start byte, invalid 12th byte, or incomplete data received.");
    }
}


void processHexString(char* hexString) {
        // Extracting bytes from the hex string
        std::vector<uint8_t> h_byte;
        std::vector<std::vector<bool>> b_byte;

        for (size_t x = 2; x < 6; x += 2) { // Process byte 1 and byte 2 (index 2 to 5)
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
