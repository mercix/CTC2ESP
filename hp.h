#include "esphome.h"
#include <sstream>
#include <cmath>

class HP : public PollingComponent, public UARTDevice {
 public:
  HP(UARTComponent *parent) : PollingComponent(15000), UARTDevice(parent) {}

  sensor::Sensor *temperature_accumulator = new sensor::Sensor();
  sensor::Sensor *temperature_outdoor_ecologic = new sensor::Sensor();
  sensor::Sensor *temperature_before_vp = new sensor::Sensor();
  sensor::Sensor *temperature_heat_gas = new sensor::Sensor();
  sensor::Sensor *temperature_outdoor_vp = new sensor::Sensor();
  sensor::Sensor *temperature_after_vp = new sensor::Sensor();

  float cand_accumulator = NAN;
  float cand_outdoor_ecologic = NAN;
  float cand_before_vp = NAN;
  float cand_heat_gas = NAN;
  float cand_outdoor_vp = NAN;
  float cand_after_vp = NAN;

  void setup() override {
  }

  void loop() override {
  }

  void update() override {
    if (available()) {
      readData();
    }
  }

  inline bool two_frame_confirm(float value, float &candidate, float eps = 0.2f) {
    if (isnan(candidate)) {
      candidate = value;
      return false;
    }

    bool ok = fabsf(value - candidate) < eps;
    candidate = value;
    return ok;
  }

  void readData() {
    const size_t maxBytes = 21;
    char hexString[(2 * maxBytes) + 1];

    char *hexPtr = hexString;
    bool validStartByte = false;
    bool valid12thByte = false;

    while ((hexPtr - hexString) < (2 * maxBytes) && available()) {
      uint8_t byte;
      if (read_byte(&byte)) {
        if ((hexPtr - hexString) == 0 && byte == 0x02) {
          validStartByte = true;
        }

        if (validStartByte) {
          hexPtr += snprintf(hexPtr, 3, "%02X", byte);

          if ((hexPtr - hexString) == 24 && byte == 0x80) {
            valid12thByte = true;
          }
        }
      }
    }

    *hexPtr = '\0';

    if (validStartByte && valid12thByte) {
      processHexString(hexString);
    }
  }

  void processHexString(const char *hexString) {
    int temp_accumulator;
    int temp_outdoor_ecologic;
    int temp_before_vp;
    int temp_heat_gas;
    int temp_outdoor_vp;
    int temp_after_vp;

    sscanf(std::string(hexString + 2, 2).c_str(), "%x", &temp_accumulator);
    sscanf(std::string(hexString + 4, 2).c_str(), "%x", &temp_outdoor_ecologic);
    sscanf(std::string(hexString + 10, 2).c_str(), "%x", &temp_before_vp);
    sscanf(std::string(hexString + 20, 2).c_str(), "%x", &temp_heat_gas);
    sscanf(std::string(hexString + 30, 2).c_str(), "%x", &temp_outdoor_vp);
    sscanf(std::string(hexString + 32, 2).c_str(), "%x", &temp_after_vp);

    temp_outdoor_ecologic = temp_outdoor_ecologic - 40;
    temp_outdoor_vp = temp_outdoor_vp - 40;

    if (two_frame_confirm(temp_accumulator, cand_accumulator)) {
      temperature_accumulator->publish_state(temp_accumulator);
    }

    if (two_frame_confirm(temp_outdoor_ecologic, cand_outdoor_ecologic)) {
      temperature_outdoor_ecologic->publish_state(temp_outdoor_ecologic);
    }

    if (two_frame_confirm(temp_before_vp, cand_before_vp)) {
      temperature_before_vp->publish_state(temp_before_vp);
    }

    if (two_frame_confirm(temp_heat_gas, cand_heat_gas)) {
      temperature_heat_gas->publish_state(temp_heat_gas);
    }

    if (two_frame_confirm(temp_outdoor_vp, cand_outdoor_vp)) {
      temperature_outdoor_vp->publish_state(temp_outdoor_vp);
    }

    if (two_frame_confirm(temp_after_vp, cand_after_vp)) {
      temperature_after_vp->publish_state(temp_after_vp);
    }
  }
};
