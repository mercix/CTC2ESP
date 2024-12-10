#include "esphome.h"
#include <sstream>


class HP : public PollingComponent, public UARTDevice {
public:
  HP(UARTComponent *parent) : PollingComponent(15000), UARTDevice(parent) {}
  
  sensor::Sensor *temperature_accumulator = new sensor::Sensor();
  sensor::Sensor *temperature_outdoor_ecologic = new sensor::Sensor();
  sensor::Sensor *temperature_before_vp = new sensor::Sensor();
  sensor::Sensor *temperature_heat_gas = new sensor::Sensor();
  sensor::Sensor *temperature_outdoor_vp = new sensor::Sensor();
  sensor::Sensor *temperature_after_vp = new sensor::Sensor();
  
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
    const size_t maxBytes = 21;
    char hexString[(2 * maxBytes) + 1];  // Adjusted buffer size and null terminator

    char* hexPtr = hexString;  // Pointer to traverse the hexString array
    bool validStartByte = false;
    bool valid12thByte = false;

    while ((hexPtr - hexString) < (2 * maxBytes) && available()) {
        uint8_t byte;
        if (read_byte(&byte)) {
            if ((hexPtr - hexString) == 0 && byte == 0x02) {
                validStartByte = true;
            }

            if (validStartByte) {
                hexPtr += snprintf(hexPtr, 3, "%02X", byte);  // Use snprintf to format and advance the pointer
                ESP_LOGD("CustomSensor", "Received byte: %02X", static_cast<unsigned>(byte));

                // Check if it's the 12th byte
                if ((hexPtr - hexString) == 24 && byte == 0x80) {
                    valid12thByte = true;
                }
            }
        }
    }

    // Null-terminate the string
    *hexPtr = '\0';

    if (validStartByte && valid12thByte && (hexPtr - hexString) > 0) {
        ESP_LOGD("CustomSensor", "Received data: %s", hexString);
        processHexString(hexString);
    } else {
        ESP_LOGD("CustomSensor", "Invalid start byte, invalid 12th byte, or incomplete data received.");
    }
}


void processHexString(char* hexString) {
    // Extracting the second byte (index 1) from the hex string
    std::string byteStringAccumulator = std::string(hexString + 2, 2);
    std::string byteStringOutdoorEcologic = std::string(hexString + 4, 2);
    std::string byteStringBeforeVp = std::string(hexString + 10, 2);
    std::string byteStringHeatGas = std::string(hexString + 20, 2);
    std::string byteStringOutdoorVp = std::string(hexString + 30, 2);
    std::string byteStringAfterVp = std::string(hexString + 32, 2);

    // Converting the extracted bytes from hex to decimal
    int temp_accumulator, temp_outdoor_ecologic, temp_before_vp, temp_heat_gas, temp_outdoor_vp, temp_after_vp;
    sscanf(byteStringAccumulator.c_str(), "%x", &temp_accumulator);
    sscanf(byteStringOutdoorEcologic.c_str(), "%x", &temp_outdoor_ecologic);
    temp_outdoor_ecologic = static_cast<int>(temp_outdoor_ecologic) - 40;
    sscanf(byteStringBeforeVp.c_str(), "%x", &temp_before_vp);
    sscanf(byteStringHeatGas.c_str(), "%x", &temp_heat_gas);
    sscanf(byteStringOutdoorVp.c_str(), "%x", &temp_outdoor_vp);
    temp_outdoor_vp = static_cast<int>(temp_outdoor_vp) - 40;
    sscanf(byteStringAfterVp.c_str(), "%x", &temp_after_vp); 

    // Output the results
    ESP_LOGD("CustomSensor", "Hex Accumulator: %s, Decimal Accumulator: %d", byteStringAccumulator.c_str(), temp_accumulator);
    ESP_LOGD("CustomSensor", "Hex Outdoor Ecologic: %s, Decimal Outdoor Ecologic: %d", byteStringOutdoorEcologic.c_str(), temp_outdoor_ecologic);
    ESP_LOGD("CustomSensor", "Hex Before VP: %s, Decimal Before VP: %d", byteStringBeforeVp.c_str(), temp_before_vp);
    ESP_LOGD("CustomSensor", "Hex Heat Gas: %s, Decimal Heat Gas: %d", byteStringHeatGas.c_str(), temp_heat_gas);
    ESP_LOGD("CustomSensor", "Hex Outdoor VP: %s, Decimal Outdoor VP: %d", byteStringOutdoorVp.c_str(), temp_outdoor_vp);
    ESP_LOGD("CustomSensor", "Hex After VP: %s, Decimal After VP: %d", byteStringAfterVp.c_str(), temp_after_vp); 

    // Check if the sensor pointers are valid
    if (temperature_accumulator && temperature_outdoor_ecologic && temperature_before_vp && temperature_heat_gas
        && temperature_outdoor_vp && temperature_after_vp) {
        ESP_LOGD("CustomSensor", "Updating temperature_accumulator sensor state");
        temperature_accumulator->publish_state(temp_accumulator);

        ESP_LOGD("CustomSensor", "Updating temperature_outdoor_ecologic sensor state");
        temperature_outdoor_ecologic->publish_state(temp_outdoor_ecologic);

        ESP_LOGD("CustomSensor", "Updating temperature_before_vp sensor state");
        temperature_before_vp->publish_state(temp_before_vp);

        ESP_LOGD("CustomSensor", "Updating temperature_heat_gas sensor state");
        temperature_heat_gas->publish_state(temp_heat_gas);

        ESP_LOGD("CustomSensor", "Updating temperature_outdoor_vp sensor state");
        temperature_outdoor_vp->publish_state(temp_outdoor_vp);

        ESP_LOGD("CustomSensor", "Updating temperature_after_vp sensor state");
        temperature_after_vp->publish_state(temp_after_vp);
    } else {
        ESP_LOGD("CustomSensor", "Error: One or more sensor pointers are null");
    }

    ESP_LOGD("CustomSensor", "Exiting processHexString");
}

  void publishSensorStates() {
    // Perform actions to publish sensor states
    // You may use ESP_LOGD for debugging
  }
};
