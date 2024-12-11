#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include <vector>

namespace esphome {
namespace dp_custom {

class DP : public PollingComponent, public uart::UARTDevice {
public:
  DP(uart::UARTComponent *parent);

  // Binary sensor declarations
  binary_sensor::BinarySensor *compressor;
  binary_sensor::BinarySensor *fan_low;
  binary_sensor::BinarySensor *fan_high;
  binary_sensor::BinarySensor *circulation_pump_hp;
  binary_sensor::BinarySensor *supplementary_heating;
  binary_sensor::BinarySensor *alarm_led;

  void setup() override;
  void update() override;
  void readData();
  void processHexString(char* hexString);
  std::vector<bool> getBits(uint8_t value);
  void publishSensorStates();
};

}  // namespace dp_custom
}  // namespace esphome
